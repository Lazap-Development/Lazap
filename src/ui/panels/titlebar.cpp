// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
// clang-format on

#include "ui/panels/titlebar.h"

#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void Titlebar::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/close.svg">(), "close",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/minimise.svg">(), "minimise",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/maximise.svg">(), "maximise",
                        0xFFFFFFFF);
  ImageManager::loadPNG(b::embed<"assets/icons/lazap/icon.png">(), "lazap");
}

void Titlebar::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar);

  ImGui::PushFont(FontManager::getFont("Titlebar:Title"));
  ImGui::SetCursorPos(
      ImVec2(ImGui::GetCursorPosX() + 35, ImGui::GetCursorPosY() + 33));
  ImGui::Text("Home");
  ImGui::PopFont();

  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 105);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(25.0f, 0.0f));
  if (ImGui::ImageButton("##minimise", ImageManager::get("minimise"),
                         ImVec2(20, 20))) {
    glfwIconifyWindow(window);
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##maximise", ImageManager::get("maximise"),
                         ImVec2(20, 20))) {
    if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
      glfwRestoreWindow(window);
    } else {
      glfwMaximizeWindow(window);
    }
  }
  ImGui::SameLine();
  if (ImGui::ImageButton("##close", ImageManager::get("close"),
                         ImVec2(20, 20))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  ImGui::PopStyleVar();
  ImGui::End();
}
