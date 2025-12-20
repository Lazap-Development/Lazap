#include "ui/panels/game_panel.h"

#include <imgui.h>

#include <algorithm>

#include "ui/panel_manager.h"
#include "ui/panels/game_box.h"
#include "ui/themes/themes.h"
#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void GamePanel::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/heart.svg">(), "heart",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart2",
                        Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2-solid.svg">(),
                        "heart2-solid", Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/recent.svg">(), "recent",
                        Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/play.svg">(), "play", 0xFFFFFFFF);
}

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

void GamePanel::render() {
  ImVec2 size = ImGui::GetMainViewport()->Size;
  ImGui::PushStyleVar(
      ImGuiStyleVar_WindowPadding,
      ImVec2(40.0, *view_ == ViewType::MainMenu ? 0 : 25.0f * size.y / 1000));
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);

  scale_ = getScale();

  ImGui::PushFont(FontManager::getFont("Title"));
  if (*view_ == ViewType::MainMenu) {
    ImGui::Text("%s", name_.c_str());
  }
  ImGui::PopFont();
  ImGui::Separator();
  ImGui::Dummy(
      ImVec2(0, ((*view_ == ViewType::MainMenu) ? 10.0f : 0.0f) * scale_.y));

  bool refreshRequested = false;
  if (!games_) {
    ImGui::PushFont(FontManager::getFont("Game:Title"));
    ImGui::TextDisabled("No games available.");
    ImGui::PopFont();
  } else {
    std::vector<std::pair<size_t, std::string>> boxIndicesWithTimestamp;

    if (*view_ == ViewType::MainMenu) {
      auto toml = storage_->loadTOML();
      auto* gamesTable = toml["games"].as_table();

      if (gamesTable) {
        for (size_t i = 0; i < gameBoxes_.size(); ++i) {
          auto& box = gameBoxes_[i];
          const std::string gameName = box->getGame().name;
          const std::string key =
              std::to_string(fnv1a::hash(gameName.c_str(), gameName.length()));

          auto* entry = gamesTable->get(key);
          if (entry && entry->is_table()) {
            std::string timestamp = entry->as_table()
                                        ->get("last_launch")
                                        ->value_or(std::string(""));

            if (!timestamp.empty()) {
              boxIndicesWithTimestamp.emplace_back(i, timestamp);
            }
          }
        }

        std::sort(
            boxIndicesWithTimestamp.begin(), boxIndicesWithTimestamp.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
      }
    } else {
      for (size_t i = 0; i < gameBoxes_.size(); ++i) {
        boxIndicesWithTimestamp.emplace_back(i, "");
      }
    }

    float boxWidth = 210.0f * scale_.x;
    float panelWidth = ImGui::GetContentRegionAvail().x;

    float horizontalSpacing = 16.0f;
    float verticalSpacing = 16.0f;

    int columns = (int)((panelWidth + horizontalSpacing) /
                        (boxWidth + horizontalSpacing));
    if (columns < 1) columns = 1;

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding,
                        ImVec2(horizontalSpacing / 2.0f, verticalSpacing));

    if (ImGui::BeginTable("games_table", columns,
                          ImGuiTableFlags_SizingStretchSame)) {
      for (const auto& [index, timestamp] : boxIndicesWithTimestamp) {
        ImGui::TableNextColumn();
        gameBoxes_[index]->render();
        if (gameBoxes_[index]->requestRefresh_) {
          refreshRequested = true;
          gameBoxes_[index]->requestRefresh_ = false;
        }
      }
      ImGui::EndTable();
    }

    ImGui::PopStyleVar();
  }

  ImGui::End();
  ImGui::PopStyleVar();

  if (refreshRequested && onRefresh_) onRefresh_();
}
