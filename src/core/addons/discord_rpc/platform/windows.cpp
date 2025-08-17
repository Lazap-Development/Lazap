#include "addons/discord_rpc/platform/windows.h"

#include <WinSock2.h>

#include <string>

namespace wine {
static bool isWine() {
  static bool wine = [] {
    auto ntdll = ::GetModuleHandleW(L"ntdll.dll");
    if (!ntdll) {
      return false;
    }
    auto func = ::GetProcAddress(ntdll, "wine_get_version");
    return func != nullptr;
  }();
  return wine;
}

static std::string getTempPath() {
  const wchar_t* envVars[] = {L"XDG_RUNTIME_DIR", L"TMPDIR", L"TMP", L"TEMP"};

  wchar_t buffer[MAX_PATH];
  for (const auto& var : envVars) {
    DWORD result = ::GetEnvironmentVariableW(var, buffer, MAX_PATH);
    if (result > 0 && result < MAX_PATH) {
      int len = WideCharToMultiByte(CP_UTF8, 0, buffer, result, nullptr, 0,
                                    nullptr, nullptr);
      std::string path(len, '\0');
      WideCharToMultiByte(CP_UTF8, 0, buffer, result, path.data(), len, nullptr,
                          nullptr);
      return path;
    }
  }

  return "/tmp";
}

static std::string convertWinePathToWindows(
    std::string const& unixPath) noexcept {
  std::string command = std::string("winepath -w \"") + unixPath + "\"";

  HANDLE hReadPipe, hWritePipe;
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};

  if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
    return "";
  }

  STARTUPINFOA si = {};
  si.cb = sizeof(STARTUPINFOA);
  si.hStdOutput = hWritePipe;
  si.hStdError = hWritePipe;
  si.dwFlags |= STARTF_USESTDHANDLES;

  PROCESS_INFORMATION pi = {};

  if (CreateProcessA(nullptr, const_cast<char*>(command.c_str()), nullptr,
                     nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si,
                     &pi)) {
    CloseHandle(hWritePipe);

    WaitForSingleObject(pi.hProcess, 5000);

    char buffer[MAX_PATH];
    DWORD bytesRead;
    std::string result;

    while (
        ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) &&
        bytesRead > 0) {
      buffer[bytesRead] = '\0';
      result += buffer;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::erase(result, '\n');
    std::erase(result, '\r');

    if (!result.empty()) {
      return result;
    }
  } else {
    CloseHandle(hWritePipe);
  }

  CloseHandle(hReadPipe);
  return "";
}
}  // namespace wine

namespace discord::platform {
size_t getProcessID() noexcept { return ::GetCurrentProcessId(); }

PipeConnection& PipeConnection::get() noexcept {
  static PipeConnection instance;
  return instance;
}

bool PipeConnection::open() noexcept {
  if (m_isOpen) {
    return false;
  }

  if (wine::isWine() && openUnix()) {
    return true;
  }

  wchar_t pipeName[] = L"\\\\?\\pipe\\discord-ipc-0";
  constexpr size_t pipeDigit = sizeof(pipeName) / sizeof(wchar_t) - 2;
  while (true) {
    m_pipe = ::CreateFileW(pipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                           OPEN_EXISTING, 0, nullptr);
    if (m_pipe != INVALID_HANDLE_VALUE) {
      m_isOpen = true;
      return true;
    }

    auto error = ::GetLastError();
    if (error == ERROR_FILE_NOT_FOUND) {
      if (pipeName[pipeDigit] < L'9') {
        pipeName[pipeDigit]++;
        continue;
      }
    } else if (error == ERROR_PIPE_BUSY) {
      if (!WaitNamedPipeW(pipeName, 10000)) {
        return false;
      }
      continue;
    }

    return false;
  }
}

bool PipeConnection::close() noexcept {
  if (m_useWineFallback) {
    return closeUnix();
  }

  if (m_pipe != INVALID_HANDLE_VALUE) {
    ::CloseHandle(m_pipe);
    m_pipe = INVALID_HANDLE_VALUE;
    m_isOpen = false;
  }
  return true;
}

bool PipeConnection::write(const void* data, size_t length) const noexcept {
  if (length == 0) {
    return true;
  }

  if (m_useWineFallback) {
    return writeUnix(data, length);
  }

  if (m_pipe == INVALID_HANDLE_VALUE) {
    return false;
  }

  if (!data) {
    return false;
  }

  const auto bytesToWrite = static_cast<DWORD>(length);
  DWORD bytesWritten = 0;
  if (!::WriteFile(m_pipe, data, bytesToWrite, &bytesWritten, nullptr)) {
    return false;
  }
  return bytesWritten == bytesToWrite;
}

bool PipeConnection::read(void* data, size_t length) noexcept {
  if (!data) {
    return false;
  }

  if (m_useWineFallback) {
    return readUnix(data, length);
  }

  if (m_pipe == INVALID_HANDLE_VALUE) {
    return false;
  }

  DWORD bytesRead = 0;
  if (!::PeekNamedPipe(m_pipe, nullptr, 0, nullptr, &bytesRead, nullptr)) {
    this->close();
    return false;
  }

  if (bytesRead < length) {
    return false;
  }

  if (!::ReadFile(m_pipe, data, length, &bytesRead, nullptr)) {
    this->close();
    return false;
  }

  return true;
}

bool PipeConnection::openUnix() noexcept {
  auto basePath = wine::getTempPath();
  if (basePath.empty()) {
    return false;
  }

  auto socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket == INVALID_SOCKET) {
    return false;
  }

  u_long mode = 1;
  ::ioctlsocket(socket, FIONBIO, &mode);

  for (int i = 0; i < 10; ++i) {
    auto socketPath = wine::convertWinePathToWindows(
        std::string(basePath) + "\\discord-ipc-" + std::to_string(i));
    if (socketPath.empty()) {
      return false;
    }

    struct UnixAddr {
      short sun_family;
      char sun_path[108] = {};
    };

    UnixAddr addr{};
    addr.sun_family = AF_UNIX;

    std::memcpy(addr.sun_path, socketPath.c_str(),
                std::min(socketPath.size(), sizeof(addr.sun_path) - 1));

    if (::connect(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) ==
        0) {
      m_pipe = reinterpret_cast<HANDLE>(socket);
      m_isOpen = true;
      m_useWineFallback = true;
      return true;
    }
  }

  return false;
}

bool PipeConnection::closeUnix() noexcept {
  auto socket = reinterpret_cast<SOCKET>(m_pipe);
  if (socket == INVALID_SOCKET) {
    return false;
  }

  if (::closesocket(socket) == SOCKET_ERROR) {
    return false;
  }

  m_pipe = INVALID_HANDLE_VALUE;
  m_isOpen = false;
  m_useWineFallback = false;

  return true;
}

bool PipeConnection::writeUnix(const void* data, size_t length) const noexcept {
  if (!data) {
    return true;
  }

  auto socket = reinterpret_cast<SOCKET>(m_pipe);
  if (socket == INVALID_SOCKET) {
    return false;
  }

  int bytesSent = ::send(socket, static_cast<const char*>(data),
                         static_cast<int>(length), 0);
  if (bytesSent == SOCKET_ERROR) {
    return false;
  }

  return bytesSent == static_cast<int>(length);
}

bool PipeConnection::readUnix(void* data, size_t length) noexcept {
  if (length == 0) {
    return true;
  }

  auto socket = reinterpret_cast<SOCKET>(m_pipe);
  if (socket == INVALID_SOCKET) {
    return false;
  }

  int bytesRead =
      ::recv(socket, static_cast<char*>(data), static_cast<int>(length), 0);
  if (bytesRead == SOCKET_ERROR) {
    return false;
  }

  return bytesRead == static_cast<int>(length);
}
}  // namespace discord::platform
