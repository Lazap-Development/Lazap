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
                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking |
                   ImGuiWindowFlags_NoTitleBar);

  ImGui::Image(ImageManager::get("lazap"), ImVec2(40, 40));

  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 130);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 2.0f));

  bool clickedMin = false, clickedMax = false, clickedClose = false;

  if (ImGui::ImageButton("##minimise", ImageManager::get("minimise"),
                         ImVec2(20, 20)))
    clickedMin = true;

  ImGui::SameLine();
  if (ImGui::ImageButton("##maximise", ImageManager::get("maximise"),
                         ImVec2(20, 20)))
    clickedMax = true;

  ImGui::SameLine();
  if (ImGui::ImageButton("##close", ImageManager::get("close"), ImVec2(20, 20)))
    clickedClose = true;

  ImGui::PopStyleVar();

  if (clickedMin) glfwIconifyWindow(window);

  if (clickedMax) {
    if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
      glfwRestoreWindow(window);
    else
      glfwMaximizeWindow(window);
  }

  if (clickedClose) glfwSetWindowShouldClose(window, GLFW_TRUE);

  {
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    float titlebarHeight = 40.0f;  // Amount of area that drags

    bool hovered = ImGui::IsMouseHoveringRect(
        pos, ImVec2(pos.x + size.x, pos.y + titlebarHeight));

    bool dragging = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    // Prevent dragging if mouse is over buttons
    bool overItem = ImGui::IsAnyItemHovered();

    if (dragging && !overItem) {
      ImVec2 delta = ImGui::GetIO().MouseDelta;

      int x, y;
      glfwGetWindowPos(window, &x, &y);
      glfwSetWindowPos(window, x + (int)delta.x, y + (int)delta.y);
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();
}
