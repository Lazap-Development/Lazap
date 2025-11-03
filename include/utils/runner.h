#pragma once

struct FpsIdling {
  float fpsIdle = 0.5f;
  bool enableIdling = true;
  bool isIdling = false;
};

struct RunnerState {
  FpsIdling fpsIdling;
};

void IdleBySleeping(FpsIdling& ioIdling);
