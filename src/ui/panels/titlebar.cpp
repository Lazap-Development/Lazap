#include "ui/panels/titlebar.h"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "utils/image_manager.h"

using namespace ui;

void Titlebar::init() {
  ImageManager::LoadSVG("src/assets/svg/close.svg", "close", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/minimise.svg", "minimise", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/maximise.svg", "maximise", 0xFFFFFFFF);
  ImageManager::LoadIcon("src/assets/icons/lazap.png");
}
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

  ImGui::Image(ImageManager::GetIcon("lazap"), ImVec2(50, 50));

  ImGui::SameLine(ImGui::GetWindowWidth() - 144);
  if (ImGui::ImageButton("##minimise", ImageManager::GetIcon("minimise"),
                         ImVec2(24, 24))) {
    glfwIconifyWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##maximise", ImageManager::GetIcon("maximise"),
                         ImVec2(24, 24))) {
    glfwMaximizeWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##close", ImageManager::GetIcon("close"),
                         ImVec2(24, 24))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  ImGui::PopStyleVar();
  ImGui::End();
}
