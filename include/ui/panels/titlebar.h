#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "ui/panel.h"

namespace ui {

class Titlebar : public Panel {
 public:
  Titlebar(GLFWwindow* w) : Panel("Titlebar"), window(w) {}

  void init() override;
  void render() override;
  void end() override {}

 private:
  GLFWwindow* window;
};

}  // namespace ui
