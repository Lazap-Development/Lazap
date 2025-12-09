#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

constexpr const int WINDOW_SIZE[2] = {1800, 1000};
// TODO: min width should be lower but we need to fix leftbar first..
constexpr const int MIN_WINDOW_SIZE[2] = {1100, 600};
constexpr const int RESIZE_BORDER = 8;

struct FpsIdling {
  float fpsIdle = 9.f;
  bool enableIdling = true;
  bool isIdling = false;
};

struct RunnerState {
  FpsIdling fpsIdling;
};

struct WindowCallbacks {
  static void mouseButtonCB(GLFWwindow* window, int button, int action,
                            int mods);
};

void IdleBySleeping(FpsIdling& ioIdling);

class Application {
 public:
  void run();
};
