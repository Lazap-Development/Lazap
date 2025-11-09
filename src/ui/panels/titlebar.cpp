#include "ui/panels/titlebar.h"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "utils/icon_manager.h"

using namespace ui;

void Titlebar::render() {
  ImGui::Begin(name().c_str(), nullptr, false);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 7));

  // Titlebar background
  ImDrawList* draw = ImGui::GetForegroundDrawList();
  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 size = ImGui::GetWindowSize();
  float rounding = 15.0f;
  float top_padding = 7.0f;
  draw->AddRectFilled(ImVec2(pos.x, pos.y + top_padding),
                      ImVec2(pos.x + size.x, pos.y + size.y + top_padding),
                      IM_COL32(0, 0, 0, 80), rounding);

  ImGui::Image(IconManager::GetIcon("lazap"), ImVec2(50, 50));

  ImGui::SameLine(ImGui::GetWindowWidth() - 144);
  if (ImGui::ImageButton("##minimise", IconManager::GetIcon("minimise"),
                         ImVec2(24, 24))) {
    glfwIconifyWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##maximise", IconManager::GetIcon("maximise"),
                         ImVec2(24, 24))) {
    glfwMaximizeWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##close", IconManager::GetIcon("close"),
                         ImVec2(24, 24))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  ImGui::PopStyleVar();
  ImGui::End();
}
