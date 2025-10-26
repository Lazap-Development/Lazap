#include "ui/panels/titlebar.h"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "utils/icon_manager.h"

using namespace ui;

void Titlebar::init() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.WindowPadding = ImVec2(0.0f, 5.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
}

void Titlebar::render() {
  if (!visible()) return;

  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet =
      ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
  ImGui::SetNextWindowClass(&window_class);
  ImGui::Begin(name().c_str(), nullptr, false);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 255));
  ImGui::Image((void*)(intptr_t)IconManager::GetIcon("lazap"), ImVec2(38, 38));

  ImGui::SameLine(ImGui::GetWindowWidth() - 144);
  if (ImGui::ImageButton("##minimise",
                         (void*)(intptr_t)IconManager::GetIcon("minimise"),
                         ImVec2(24, 24))) {
    glfwIconifyWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##maximise",
                         (void*)(intptr_t)IconManager::GetIcon("maximise"),
                         ImVec2(24, 24))) {
    glfwMaximizeWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##close",
                         (void*)(intptr_t)IconManager::GetIcon("close"),
                         ImVec2(24, 24))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  ImGui::PopStyleColor();
  ImGui::End();
}
