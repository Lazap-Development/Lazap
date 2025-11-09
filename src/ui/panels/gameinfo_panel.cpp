// clang-format off
#include "ui/panels/gameinfo_panel.h"
// clang-format on
#include "imgui.h"

using namespace ui;

void GameInfoPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::Separator();
  ImGui::End();
}
