#include "launch_manager.h"

#include <cstdlib>
#include <thread>

bool LaunchManager::launchGame(const std::string &command,
                               const std::string &args) {
#ifdef _WIN32
  std::thread([command, args] {
    std::system(("start " + args).c_str());
  }).detach();
  return true;

#elif defined(__linux__) || defined(__APPLE__)
  std::thread([command, args] {
    std::system((command + " " + args).c_str());
  }).detach();
  return true;

#else
  return false;
#endif
}

// TODO: figure out a cross-platform way to detect if game process is running
bool isRunning() { return true; }
