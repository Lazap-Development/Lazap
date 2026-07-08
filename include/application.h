#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

constexpr const int WINDOW_SIZE[2] = {1800, 1000};
constexpr const int MIN_WINDOW_SIZE[2] = {800, 600};
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
#ifdef _WIN32
  static GLFWcursor* cursor_nwse;
  static GLFWcursor* cursor_nesw;
  static GLFWcursor* cursor_h;
  static GLFWcursor* cursor_v;
  static bool hovered_;

  static int GetResizeZone(double xpos, double ypos, int w, int h,
                           int threshold) {
    bool l = xpos <= threshold;
    bool r = xpos >= w - threshold;
    bool t = ypos <= threshold;
    bool b = ypos >= h - threshold;

    if (t && l) return 1;  // Top-Left
    if (t && r) return 2;  // Top-Right
    if (b && l) return 3;  // Bottom-Left
    if (b && r) return 4;  // Bottom-Right
    if (l) return 5;       // Left
    if (r) return 6;       // Right
    if (t) return 7;       // Top
    if (b) return 8;       // Bottom
    return 0;              // None
  }
  static void cursorPosCB(GLFWwindow* window, double xpos, double ypos);
  static void initCursors() {
    cursor_nwse = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    cursor_nesw = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    cursor_h = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_v = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
  }
#endif
};

void IdleBySleeping(FpsIdling& ioIdling);

class Application {
 public:
  void run();
};
