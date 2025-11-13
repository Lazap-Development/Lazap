// clang-format off
#include "ui/panels/gameinfo_panel.h"
// clang-format on

#include "battery/embed.hpp"
#include "imgui.h"
#include "utils/image_manager.h"

using namespace ui;

void GameInfoPanel::init() {
  ImageManager::loadPNG(b::embed<"assets/img/banner.png">(), "banner");
}

void GameInfoPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::Separator();
  ImGui::End();
}
