#include "ui/panels/game_panel.h"

#include "imgui.h"
#include "ui/panels/game_box.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void GamePanel::setGames(const std::vector<Game>* games) { games_ = games; }

void GamePanel::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart2",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/recent.svg">(), "recent",
                        0xA2A2A2FF);
  ImageManager::loadSVG(b::embed<"assets/svg/play.svg">(), "play", 0xFFFFFFFF);
}

void GamePanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);

  ImGui::PushFont(FontManager::getFont("Title"));
  ImGui::Text(name_.c_str());
  ImGui::PopFont();
  ImGui::Separator();

  if (!games_) {
    ImGui::PushFont(FontManager::getFont("Game:Title"));
    ImGui::TextDisabled("No games available.");
    ImGui::PopFont();
  } else {
    float boxWidth = 210.0f + 12.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = (int)(panelWidth / boxWidth);
    if (columns < 1) columns = 1;

    if (ImGui::BeginTable("games_table", columns,
                          ImGuiTableFlags_SizingFixedFit)) {
      for (const auto& game : *games_) {
        ImGui::TableNextColumn();
        GameBox box(game, storage_);
        box.render();
      }
      ImGui::EndTable();
    }
  }
  ImGui::End();
}
