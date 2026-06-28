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
  ImVec2 getScale() {
    ImGuiIO& io = ImGui::GetIO();
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      return ImVec2(SCALE_FACTOR, SCALE_FACTOR);
    }
    return ImVec2(io.DisplayFramebufferScale.x * SCALE_FACTOR * xs_,
                  io.DisplayFramebufferScale.y * SCALE_FACTOR * ys_);
  }
  float getHeight() { return size_.y; }

 private:
  bool customTitlebar_ = true;

  std::string title_ = "Home";
  GLFWwindow* window;
  ImVec2 size_;
};

}  // namespace ui
