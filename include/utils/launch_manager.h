#pragma once
#include <clients/client.h>

class LaunchManager {
 public:
  LaunchManager(const Game& game) : game_(game) {}

  bool launch();
  bool isRunning();
  bool kill();

 private:
  const Game& game_;
  int pid_ = -1;
};
