#pragma once
#include <GLFW/glfw3.h>

#include "ui/panel.h"
#include "ui/panel_manager.h"

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
