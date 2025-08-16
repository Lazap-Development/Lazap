
#pragma once
#include <string>

class LaunchManager {
 public:
  LaunchManager(const std::string &installPath, const std::string &executable);

  bool launch();
  bool isRunning();
  bool kill();

 private:
  std::string installPath_;
  std::string executable_;
  int pid_ = -1;
};
