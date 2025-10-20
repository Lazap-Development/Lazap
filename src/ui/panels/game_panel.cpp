#include "ui/panels/game_panel.h"
#include "imgui.h"
#include "utils/launch_manager.h"
#include <string>

using namespace ui;

void GamePanel::init() {
  ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.Fonts->AddFontFromFileTTF("src/assets/fonts/Nunito-Medium.ttf", 24.0f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 8.0f;
	style.FrameRounding = 5.0f;
	style.FramePadding = ImVec2(8.0f, 6.0f);

}

void GamePanel::render() {
  // ImGui::SetWindowSize(ImVec2(400.0f, 800.0f));
  // ImGui::SetWindowPos(ImVec2(208.0f, 8.0f));

  ImGui::Begin("Game Menu", nullptr, ImGuiWindowFlags_NoCollapse |
  // ImGuiWindowFlags_NoResize |
  // ImGuiWindowFlags_NoMove |
  ImGuiWindowFlags_NoScrollbar |
  ImGuiWindowFlags_NoTitleBar);

  ImGui::Text("All Games");
  ImGui::Separator();
  if (!games_) {
    ImGui::TextDisabled("No games available.");
    ImGui::End();
    return;
  }
  ImGui::End();
}

void GamePanel::definePointers() {
  games_ = games;
}