#pragma once
#include <string>

class LaunchManager {
 public:
  static bool launchGame(const std::string &command, const std::string &args);
  static bool isRunning(int pid);
};
