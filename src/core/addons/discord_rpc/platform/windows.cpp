#ifdef _WIN32

#include <WinSock2.h>

#include <string>

#include "addons/discord_rpc/platform.h"

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

}  // namespace discord::platform
#endif  // _WIN32
