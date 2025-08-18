#include <clients/client.h>
#include <clients/steam/steam.h>
#include <utils/launch_manager.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>

#include <tlhelp32.h>
#else
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

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

  std::string fullPath =
      (fs::path(game_.installPath) / game_.executable).string();

  if (!fs::exists(fullPath)) {
    std::string foundPath =
        findExecutableIgnoreCase(game_.installPath, game_.executable);
    if (foundPath.empty()) {
      return false;
    }
    fullPath = foundPath;
  }

#ifdef _WIN32
  STARTUPINFO si = {sizeof(STARTUPINFO)};
  PROCESS_INFORMATION pi;

  if (!CreateProcess(fullPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL,
                     game_.installPath.c_str(), &si, &pi)) {
    return false;
  }
  pid_ = pi.dwProcessId;
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
#else
  pid_ = fork();
  if (pid_ == 0) {
    switch (game_.clientType) {
      case ClientType::Steam: {
        // TODO: Don't set hardcoded proton path
        std::string protonPath =
            "/home/porya/.steam/steam/compatibilitytools.d/Proton-GE "
            "Latest/proton";

        setenv("STEAM_COMPAT_DATA_PATH",
               (fs::path(game_.installPath).parent_path().parent_path() /
                "compatdata" / std::to_string(game_.appId))
                   .string()
                   .c_str(),
               1);

        setenv("STEAM_COMPAT_CLIENT_INSTALL_PATH", Steam::getLocation().c_str(),
               1);

        chdir(game_.installPath.c_str());

        execl(protonPath.c_str(), protonPath.c_str(), "run", fullPath.c_str(),
              (char*)nullptr);
      }
        std::_Exit(EXIT_FAILURE);
    }
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
