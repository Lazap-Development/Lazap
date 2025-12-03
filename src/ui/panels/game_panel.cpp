#include "ui/panels/game_panel.h"

#include "imgui.h"
#include "ui/panel_manager.h"
#include "ui/panels/game_box.h"
#include "ui/themes/themes.h"
#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void GamePanel::setGames(const std::vector<Game>* games) {
  games_ = games;
  gameBoxes_.clear();

  if (!games_) return;

  auto toml = storage_->loadTOML();
  auto* gamesTable = toml["games"].as_table();
  if (!gamesTable) return;

  for (const auto& game : *games_) {
    const std::string key = std::to_string(
        fnv1a::hash(game.name.c_str(), std::strlen(game.name.c_str())));

    auto* entry = gamesTable->get(key);
    if (!entry || !entry->is_table()) continue;

    bool shouldAdd = true;

    if (*view_ == ViewType::Favorites) {
      shouldAdd = entry->as_table()->get("favourite")->value_or(false);
    }

    if (shouldAdd) {
      gameBoxes_.emplace_back(std::make_unique<GameBox>(game, storage_));
    }
  }
}

void GamePanel::init() {
  ImageManager::loadSVG(b::embed("assets/svg/heart2.svg"), "heart2",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed("assets/svg/recent.svg"), "recent",
                        0xA2A2A2FF);
  ImageManager::loadSVG(b::embed("assets/svg/play.svg"), "play", 0xFFFFFFFF);
}

void GamePanel::render() {
  ImVec2 size = ImGui::GetMainViewport()->Size;
  ImGui::PushStyleVar(
      ImGuiStyleVar_WindowPadding,
      ImVec2(35.0f * size.x / 1800,
             *view_ == ViewType::MainMenu ? 0.0f : 37.0f * size.y / 1000));
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  if (scale_.x == 0) {
    scale_ = Themes::getScale(1685, *view_ == ViewType::MainMenu ? 340 : 893);
  }
  ImGui::PushFont(FontManager::getFont("Title"));
  ImGui::Text("%s", name_.c_str());
  ImGui::PopFont();
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 30 * scale_.y));

  bool refreshRequested = false;

  if (!games_) {
    ImGui::PushFont(FontManager::getFont("Game:Title"));
    ImGui::TextDisabled("No games available.");
    ImGui::PopFont();
  } else {
    float boxWidth = 210.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x + 40;
    int columns = (int)(panelWidth / boxWidth);
    if (columns < 1) columns = 1;

    if (ImGui::BeginTable("games_table", columns,
                          ImGuiTableFlags_SizingStretchSame)) {
      for (auto& box : gameBoxes_) {
        ImGui::TableNextColumn();
        box->render();
        if (box->requestRefresh_) {
          refreshRequested = true;
          box->requestRefresh_ = false;
        }
      }
      ImGui::EndTable();
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();

  if (refreshRequested && onRefresh_) onRefresh_();
}
