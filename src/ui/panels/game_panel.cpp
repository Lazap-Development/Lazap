#include "ui/panels/game_panel.h"

#include <string>

#include "imgui.h"
#include "iostream"
#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/launch_manager.h"
using namespace ui;

void GamePanel::init() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
}

void GamePanel::setGames(const std::vector<Game>* games) { games_ = games; }

void GamePanel::render() {
  if (!visible()) return;

  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);

  ImGui::PushFont(FontManager::GetFont("Title"));
  ImGui::Text(name_.c_str());
  ImGui::PopFont();
  ImGui::Separator();

  if (!games_) {
    ImGui::PushFont(FontManager::GetFont("Game:Title"));
    ImGui::TextDisabled("No games available.");
    ImGui::PopFont();
  } else {
    for (const auto& game : *games_) {
      LaunchManager lm = LaunchManager(game);
      if (ImGui::Button(game.name.c_str())) lm.launch();
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        storage_->updateTOML([game](toml::table& config) {
          if (config.contains("games")) {
            auto gamesTable = config["games"].as_table();
            if (gamesTable->contains(std::to_string(fnv1a::hash(
                    game.name.c_str(), std::strlen(game.name.c_str()))))) {
              auto gameTable =
                  gamesTable
                      ->at(std::to_string(fnv1a::hash(
                          game.name.c_str(), std::strlen(game.name.c_str()))))
                      .as_table();
              gameTable->insert_or_assign("favourite", toml::value(true));
            }
          }
        });
        if (lm.isRunning()) {
          lm.kill();
        }
      }
    }
  }

  ImGui::End();
}
