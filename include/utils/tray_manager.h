#pragma once

#include "GLFW/glfw3.h"

namespace TrayManager {
void init(GLFWwindow* window);
void update();
void minimize();
bool shouldQuit();
bool isWindowHidden();
void shutdown();
}  // namespace TrayManager