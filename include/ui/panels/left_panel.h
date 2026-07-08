#pragma once
#include "ui/panel.h"
#include "ui/panel_manager.h"

namespace ui {

class LeftPanel : public Panel {
 public:
  LeftPanel(Views* v) : Panel("Left Menu"), view_(v) {}

  void init() override;
  void render() override;
  void end() override {}
  ImVec2 getScale() {
    ImGuiIO& io = ImGui::GetIO();
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      return ImVec2(SCALE_FACTOR, SCALE_FACTOR);
    }
    return ImVec2(io.DisplayFramebufferScale.x * SCALE_FACTOR * xs_,
                  io.DisplayFramebufferScale.y * SCALE_FACTOR * ys_);
  }
  float getWidth() { return size_.x; }

 private:
  Views* view_ = nullptr;
  ImVec2 size_;
};

}  // namespace ui
