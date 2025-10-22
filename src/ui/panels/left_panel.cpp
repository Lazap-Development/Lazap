#include "ui/panels/left_panel.h"

#include "imgui.h"

using namespace ui;

void LeftPanel::init() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("src/assets/fonts/Nunito-Medium.ttf", 24.0f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
}

void LeftPanel::render() {
  // ImGui::SetWindowPos(ImVec2(8.0f, 8.0f));
  // ImGui::SetWindowSize(ImVec2(200.0f, 800.0f));

  ImGui::Begin("Left Panel", nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   //  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
  ImGui::Button("Home");
  ImGui::Button("Recently Played");
  ImGui::Button("All Games");
  ImGui::Button("Favorites");
  ImGui::Separator();
  ImGui::End();
}
