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

  ImGui::PushFont(FontManager::GetFont("Title"));
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::PopFont();

  // ImDrawList* draw_list = ImGui::GetWindowDrawList();
  // ImVec2 panel_pos = ImGui::GetWindowPos();
  // ImVec2 panel_size = ImGui::GetWindowSize();
  // draw_list->AddImage(
  //     (void*)(intptr_t)IconManager::GetIcon("banner"), ImVec2(0, 0),
  //     ImVec2(panel_pos.x + panel_size.x, panel_pos.y + panel_size.y));
  ImGui::Separator();
  ImGui::End();
}
