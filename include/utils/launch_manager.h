#pragma once
#include <string>

#include "../clients/client.h"

class LaunchManager {
 public:
  static bool launch(const ClientType &client, const std::string &installPath,
                     const std::string &executable);
  static bool isRunning(int pid);
};
