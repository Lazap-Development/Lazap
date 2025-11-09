// clang-format off
#include "GL/glew.h"
#include "ui/panels/gameinfo_panel.h"
// clang-format on
#include "imgui.h"
#include "utils/font_manager.h"
#include "utils/icon_manager.h"

using namespace ui;

void GameInfoPanel::init() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
}

void GameInfoPanel::render() {
  if (!visible()) return;

  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::Separator();
  ImGui::End();
}
