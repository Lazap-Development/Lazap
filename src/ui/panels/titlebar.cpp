// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
// clang-format on

#include "ui/panels/titlebar.h"

#include "utils/image_manager.h"

using namespace ui;

void Titlebar::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/close.svg">(), "close",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/minimise.svg">(), "minimise",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/maximise.svg">(), "maximise",
                        0xFFFFFFFF);
  ImageManager::loadPNG(b::embed<"assets/icons/lazap.png">(), "lazap");
}

void Titlebar::render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

  ImGui::Begin(name().c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar);

  // Titlebar background
  // ImDrawList* draw = ImGui::GetForegroundDrawList();
  // ImVec2 pos = ImGui::GetWindowPos();
  // ImVec2 size = ImGui::GetWindowSize();
  // float rounding = 15.0f;
  // float top_padding = 7.0f;
  // draw->AddRectFilled(ImVec2(pos.x, pos.y + top_padding),
  //                     ImVec2(pos.x + size.x, pos.y + size.y + top_padding),
  //                     IM_COL32(0, 0, 0, 80), rounding);

  ImGui::Image(ImageManager::get("lazap"), ImVec2(40, 40));
  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 130);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 2.0f));
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

  ImGui::PopStyleVar();
}
