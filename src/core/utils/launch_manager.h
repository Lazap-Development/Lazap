#pragma once
#include <string>

#include "../clients/client.h"

class LaunchManager {
 public:
  static bool launchGame(const ClientType &client,
                         const std::string &executable);
  static bool isRunning(int pid);
};
