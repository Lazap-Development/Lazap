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

  void setTitle(const std::string& text) { title_ = text; }
  void setCustomTitlebar(bool enabled) { customTitlebar_ = enabled; }

 private:
  bool customTitlebar_ = true;

  std::string title_ = "Home";
  GLFWwindow* window;
};

}  // namespace ui
