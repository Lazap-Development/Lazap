#include "ui/panels/game_panel.h"

#include "imgui.h"
#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/launch_manager.h"

using namespace ui;

void GamePanel::setGames(const std::vector<Game>* games) { games_ = games; }

void GamePanel::render() {
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
