#pragma once
#include <glad/glad.h>

#include <string>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "storage/storage.h"

namespace ui {

static constexpr float SCALE_FACTOR = 0.8f;

class Panel {
 public:
  Panel(const std::string& name = "Panel", Storage* storage = nullptr)
      : name_(name), storage_(storage) {}
  virtual ~Panel() = default;

  virtual void init() {}
  virtual void render() = 0;
  virtual void end() {}

  const std::string& getName() const { return name_; }
  bool visible() const { return visible_; }
  void setVisible(bool v) { visible_ = v; }

  void setOnRefresh(std::function<void()> cb) { onRefresh_ = cb; }

  static ImVec2 getScale() {
    ImGuiIO& io = ImGui::GetIO();
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      return ImVec2(SCALE_FACTOR, SCALE_FACTOR);
    }
    return ImVec2(io.DisplayFramebufferScale.x * SCALE_FACTOR,
                  io.DisplayFramebufferScale.y * SCALE_FACTOR);
  }

 protected:
  std::string name_;
  Storage* storage_;
  bool visible_ = true;
  ImVec2 scale_ = ImVec2(0, 0);

  std::function<void()> onRefresh_;
};

}  // namespace ui
