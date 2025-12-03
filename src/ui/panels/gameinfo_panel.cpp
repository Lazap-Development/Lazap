// clang-format off
#include "ui/panels/gameinfo_panel.h"
// clang-format on

#include "battery/embed.hpp"
#include "imgui.h"
#include "utils/image_manager.h"

using namespace ui;

void GameInfoPanel::init() {
  ImageManager::loadPNG(b::embed("assets/img/banner.png"), "banner");
}

void GameInfoPanel::render() {
  ImVec2 size = ImGui::GetMainViewport()->Size;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                      ImVec2(35.0f * size.x / 1800, 0));
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::Separator();
  ImGui::End();
  ImGui::PopStyleVar();
}
