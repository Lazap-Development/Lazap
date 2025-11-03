#pragma once
#include "GL/glew.h"

struct FpsIdling {
  float fpsIdle = 9.f;
  bool enableIdling = true;
  bool isIdling = false;
};

struct RunnerState {
  FpsIdling fpsIdling;
};

void IdleBySleeping(FpsIdling& ioIdling);

class Application {
 public:
  void run();
};
