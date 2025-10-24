#include "ui/panels/game_panel.h"

#include <string>

#include "imgui.h"
#include "utils/launch_manager.h"

using namespace ui;

void GamePanel::init() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("src/assets/fonts/Nunito-Medium.ttf", 24.0f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
}

void GamePanel::render() {
  if (!visible()) return;

  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);

  ImGui::Text("All Games");
  ImGui::Separator();
  if (!games_) {
    ImGui::TextDisabled("No games available.");
    ImGui::End();
    return;
  }
  // for (auto &game : games_) {
  //   LaunchManager lm = LaunchManager(game);
  //   if (ImGui::Button(game.name.c_str())) lm.launch();

  //   if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
  //     if (lm.isRunning()) {
  //       lm.kill();
  //     }
  //   }
  // }

  ImGui::End();
}

void GamePanel::definePointers() { games_ = games; }