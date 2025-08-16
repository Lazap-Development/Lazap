#include <utils/launch_manager.h>

#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
#include <tlhelp32.h>
#include <windows.h>
#else
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

LaunchManager::LaunchManager(const std::string& installPath,
                             const std::string& executable)
    : installPath_(installPath), executable_(executable) {}

std::string findExecutableIgnoreCase(const std::string& directory,
                                     const std::string& execName) {
  namespace fs = std::filesystem;

  if (!fs::exists(directory) || !fs::is_directory(directory)) {
    return "";
  }

  std::string lowerTarget = execName;
  std::transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(),
                 ::tolower);

  try {
    for (const auto& entry : fs::directory_iterator(directory)) {
      if (entry.is_regular_file()) {
        std::string filename = entry.path().filename().string();
        std::string lowerFilename = filename;
        std::transform(lowerFilename.begin(), lowerFilename.end(),
                       lowerFilename.begin(), ::tolower);

        if (lowerFilename == lowerTarget) {
          return entry.path().string();
        }
      }
    }
  } catch (const fs::filesystem_error&) {
    return "";
  }

  return "";
}

bool LaunchManager::launch() {
  namespace fs = std::filesystem;
  std::string fullPath = (fs::path(installPath_) / executable_).string();

  if (!fs::exists(fullPath)) {
    std::string foundPath = findExecutableIgnoreCase(installPath_, executable_);
    if (foundPath.empty()) {
      return false;
    }
    fullPath = foundPath;
  }

#ifdef _WIN32
  STARTUPINFO si = {sizeof(STARTUPINFO)};
  PROCESS_INFORMATION pi;

  if (!CreateProcess(fullPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL,
                     installPath_.c_str(), &si, &pi)) {
    return false;
  }
  pid_ = pi.dwProcessId;
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
#else
  pid_ = fork();
  if (pid_ == 0) {
    execl(fullPath.c_str(), fullPath.c_str(), (char*)nullptr);
    std::_Exit(EXIT_FAILURE);
  } else if (pid_ < 0) {
    return false;
  }
#endif
  return true;
}

bool LaunchManager::isRunning() {
  if (pid_ <= 0) return false;

#ifdef _WIN32
  HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid_);
  if (process == NULL) return false;
  DWORD ret = WaitForSingleObject(process, 0);
  CloseHandle(process);
  return ret == WAIT_TIMEOUT;
#else
  return ::kill(pid_, 0) == 0;
#endif
}

bool LaunchManager::kill() {
  if (pid_ <= 0) return false;

#ifdef _WIN32
  HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid_);
  if (process == NULL) return false;
  bool success = TerminateProcess(process, 1);
  CloseHandle(process);
  return success;
#else
  return ::kill(pid_, SIGTERM) == 0;
#endif
}
