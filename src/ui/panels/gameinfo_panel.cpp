#include "ui/panels/gameinfo_panel.h"

#include "imgui.h"

using namespace ui;

void GameInfoPanel::init() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("src/assets/fonts/Nunito-Medium.ttf", 36.0f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
}

void GameInfoPanel::render() {
  if (!visible()) return;

  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::TextDisabled("Work In Progress...");
  ImGui::Separator();
  ImGui::End();
}
