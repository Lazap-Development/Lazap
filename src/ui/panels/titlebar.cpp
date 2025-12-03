// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
// clang-format on

#include "ui/panels/titlebar.h"

#include "ui/themes/themes.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void Titlebar::init() {
  ImageManager::loadSVG(b::embed("assets/svg/close.svg"), "close", 0xFFFFFFFF);
  ImageManager::loadSVG(b::embed("assets/svg/minimise.svg"), "minimise",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed("assets/svg/maximise.svg"), "maximise",
                        0xFFFFFFFF);
}

void Titlebar::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar);

  if (scale_.x == 0) {
    scale_ = Themes::getScale(1720, 70);
  }
  ImGui::PushFont(FontManager::getFont("Titlebar:Title"));
  ImGui::SetCursorPos(ImVec2((ImGui::GetCursorPosX() + 35) * scale_.x,
                             (ImGui::GetCursorPosY() + 33) * scale_.y));
  ImGui::Text("Home");
  ImGui::PopFont();

  ImGui::SameLine(ImGui::GetContentRegionAvail().x - (143 * scale_.x));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                      ImVec2(25.0f * scale_.x, 0.0f));
  ImGui::SetCursorPos(
      ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (11 * scale_.y)));
  if (ImGui::ImageButton("##minimise", ImageManager::get("minimise"),
                         ImVec2(24 * scale_.x, 24 * scale_.x))) {
    glfwIconifyWindow(window);
  }
  ImGui::SameLine();
  ImGui::SetCursorPos(
      ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (11 * scale_.y)));
  if (ImGui::ImageButton("##maximise", ImageManager::get("maximise"),
                         ImVec2(24 * scale_.x, 24 * scale_.x))) {
    if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
      glfwRestoreWindow(window);
    } else {
      glfwMaximizeWindow(window);
    }
  }
  ImGui::SameLine();
  ImGui::SetCursorPos(
      ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - (11 * scale_.y)));
  if (ImGui::ImageButton("##close", ImageManager::get("close"),
                         ImVec2(24 * scale_.x, 24 * scale_.x))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  ImGui::PopStyleVar();

  if (ImGui::IsWindowHovered() &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    if (!ImGui::IsAnyItemHovered()) {
      glfwDragWindow(window);
    }
  }

  ImGui::End();
}
