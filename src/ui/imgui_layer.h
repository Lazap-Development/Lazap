#pragma once
#include <cstdio>
#include "GLFW/glfw3.h"

class ImGuiLayer {
public:
    void init(GLFWwindow* window);
    void begin();
    void render();
    void end();
    void shutdown();
};

