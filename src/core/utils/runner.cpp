#include "utils/runner.h"

#include <chrono>

#include "GLFW/glfw3.h"

double ClockSeconds() {
  using Clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<double>;
  static const auto start = Clock::now();
  return std::chrono::duration_cast<second>(Clock::now() - start).count();
}

void IdleBySleeping(FpsIdling& ioIdling) {
  ioIdling.isIdling = false;
  if ((ioIdling.fpsIdle > 0.f) && ioIdling.enableIdling) {
    time_t beforeWait = time(0);
    double waitTimeout = 1. / (double)ioIdling.fpsIdle;

    // Backend specific call that will wait for an event for a maximum duration
    // of waitTimeout (for example glfwWaitEventsTimeout(timeout_seconds))
    glfwWaitEventsTimeout(waitTimeout);

    time_t afterWait = time(0);
    double waitDuration = difftime(afterWait, beforeWait);
    double waitIdleExpected = 1. / ioIdling.fpsIdle;
    ioIdling.isIdling = (waitDuration > waitIdleExpected * 0.9);
  }
}
