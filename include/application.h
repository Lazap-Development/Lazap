#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

constexpr const int WINDOW_SIZE[2] = {1800, 1000};
constexpr const int MIN_WINDOW_SIZE[2] = {800, 600};
constexpr const int RESIZE_BORDER = 8;

// FYI: bitmask flags representing window edges for resize operations
enum ResizeEdge { NONE = 0, LEFT = 1, RIGHT = 2, TOP = 4, BOTTOM = 8 };

struct ResizeState {
  int edge = NONE;
  double startScreenX, startScreenY;
  int startWidth, startHeight;
  int startPosX, startPosY;
  bool isDragging = false;
  GLFWcursor* cursorArrow = nullptr;
  GLFWcursor* cursorResize_NS = nullptr;
  GLFWcursor* cursorResize_EW = nullptr;
  GLFWcursor* cursorResize_NESW = nullptr;
  GLFWcursor* cursorResize_NWSE = nullptr;
};

struct FpsIdling {
  float fpsIdle = 9.f;
  bool enableIdling = true;
  bool isIdling = false;
};

struct RunnerState {
  FpsIdling fpsIdling;
};

struct WindowCallbacks {
  static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                    int mods);
  static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};

void IdleBySleeping(FpsIdling& ioIdling);

class Application {
 public:
  void run();
};
