#include <utils/launch_manager.h>

#include <cstdlib>
#include <thread>

bool LaunchManager::launchGame(const ClientType &client,
                               const std::string &executable) {
#ifdef _WIN32
  // Windows - start process via 'start' command in a thread
  std::thread([executable] {
    // Wrap in quotes if path contains spaces
    std::string cmd = "start \"\" \"" + executable + "\"";
    std::system(cmd.c_str());
  }).detach();
  return true;

#elif defined(__linux__) || defined(__APPLE__)
  std::thread([executable] {
    // Check if executable ends with ".exe"
    if (executable.size() >= 4 &&
        executable.compare(executable.size() - 4, 4, ".exe") == 0) {
      // Could attempt to run with Wine or Proton if available, otherwise
      // fallback
      std::string wineCmd = "wine \"" + executable + "\"";
      std::system(wineCmd.c_str());
    } else {
      // Normal executable
      std::system(("\"" + executable + "\"").c_str());
    }
  }).detach();
  return true;

#else
  return false;
#endif
}
