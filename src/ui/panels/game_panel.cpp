#include "ui/panels/game_panel.h"

#include <imgui.h>

#include <algorithm>
#include <cstring>
#include <unordered_set>

#include "clients/custom_games.h"
#include "ui/cmps/addgame_dialog.h"
#include "ui/cmps/game_box.h"
#include "ui/panel_manager.h"
#include "ui/theme.h"
#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void GamePanel::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/search.svg">(), "search",
                        0xFFFFFFFF);

  ImageManager::loadSVG(b::embed<"assets/svg/heart.svg">(), "heart",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart2",
                        Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2-solid.svg">(),
                        "heart2-solid", Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/recent.svg">(), "recent",
                        Themes::ACCENT_COLOR);
  ImageManager::loadSVG(b::embed<"assets/svg/play.svg">(), "play", 0xFFFFFFFF);

  FontManager::loadFont("CustomGame:Plus",
                        b::embed<"assets/fonts/Oxanium-ExtraLight.ttf">(),
                        64.0f);

  addGameDialog_ = std::make_unique<AddGameDialog>(storage_);

  std::memset(searchBuffer_, 0, sizeof(searchBuffer_));
}

void GamePanel::setGames(const std::vector<Game>* games) {
  games_ = games;
  gameBoxes_.clear();
  if (!games_) return;

  auto toml = storage_->loadTOML();
  auto* gamesTable = toml["games"].as_table();
  if (!gamesTable) return;

  std::unordered_set<uint64_t> processedGameIds;

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
      processedGameIds.insert(game.appId);
    }
  }

  CustomGames customGames(*storage_);
  std::vector<Game> customGamesList = customGames.getInstalledGames();

  for (const auto& game : customGamesList) {
    if (processedGameIds.count(game.appId) > 0) continue;

    const std::string key = std::to_string(game.appId);

    if (!gamesTable->contains(key)) {
      toml::table newEntry;
      newEntry.insert("favourite", false);
      gamesTable->insert(key, newEntry);
    }

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
      ImVec2(30.0, *view_ == ViewType::MainMenu ? 0 : 25.0f * size.y / 1000));
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

  if (*view_ != ViewType::MainMenu) {
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::PushFont(FontManager::getFont("GameBox:Title"));

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 oldPadding = style.FramePadding;
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 newPadding = ImVec2(38, 8);
    style.FramePadding = newPadding;

    ImGui::InputTextWithHint("##search", "Search...", searchBuffer_,
                             IM_ARRAYSIZE(searchBuffer_));

    ImVec2 itemSize = ImGui::GetItemRectSize();

    Themes::drawInputBorder();

    ImVec2 iconPos =
        ImVec2(cursorPos.x + 15, cursorPos.y + (itemSize.y - 12) / 2);
    ImGui::SetCursorScreenPos(iconPos);
    ImGui::Image(ImageManager::get("search"), ImVec2(12, 12));

    style.FramePadding = oldPadding;
    ImGui::PopFont();
    ImGui::PopItemWidth();
    ImGui::Dummy(ImVec2(0, 30.0f * scale_.y));
  }

  bool refreshRequested = false;
  if (!games_) {
    ImGui::PushFont(FontManager::getFont("Game:Title"));
    ImGui::TextDisabled("No games available.");
    ImGui::PopFont();
  } else {
    std::vector<std::pair<size_t, std::string>> boxIndicesWithTimestamp;

    auto matchesSearch = [this](const std::string& gameName) -> bool {
      if (std::strlen(searchBuffer_) == 0) return true;

      std::string lowerName = gameName;
      std::string lowerSearch = searchBuffer_;
      std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                     ::tolower);
      std::transform(lowerSearch.begin(), lowerSearch.end(),
                     lowerSearch.begin(), ::tolower);

      return lowerName.find(lowerSearch) != std::string::npos;
    };

    if (*view_ == ViewType::MainMenu) {
      auto toml = storage_->loadTOML();
      auto* gamesTable = toml["games"].as_table();

      if (gamesTable) {
        for (size_t i = 0; i < gameBoxes_.size(); ++i) {
          auto& box = gameBoxes_[i];
          const std::string gameName = box->getGame().name;

          if (!matchesSearch(gameName)) continue;

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
        const std::string gameName = gameBoxes_[i]->getGame().name;

        if (!matchesSearch(gameName)) continue;

        boxIndicesWithTimestamp.emplace_back(i, "");
      }
    }

    float boxWidth = 210.0f * scale_.x;
    float panelWidth = ImGui::GetContentRegionAvail().x;

    float horizontalSpacing = 12.0f;
    float verticalSpacing = 14.0f;

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

      if (*view_ == ViewType::Library) {
        ImGui::TableNextColumn();
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(210 * scale_.x, 233.1 * scale_.y);

        draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                      IM_COL32(162, 162, 162, 120), 5.25f, 0, 2.0f);

        ImGui::InvisibleButton("hitbox", size);

        if (ImGui::IsItemHovered()) {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (ImGui::IsItemClicked()) {
          addGameDialog_->open();
        }

        if (addGameDialog_) {
          addGameDialog_->render();
          if (addGameDialog_->requestRefresh_) {
            refreshRequested = true;
            addGameDialog_->requestRefresh_ = false;
          }
        }

        ImGui::PushFont(FontManager::getFont("CustomGame:Plus"));
        ImVec2 text_size = ImGui::CalcTextSize("+");
        ImVec2 text_pos = ImVec2(pos.x + (size.x - text_size.x) * 0.5f,
                                 pos.y + (size.y - text_size.y) * 0.5f);
        draw->AddText(text_pos, IM_COL32(162, 162, 162, 255), "+");
        ImGui::PopFont();
      }

      ImGui::EndTable();
    }

    ImGui::PopStyleVar();
  }

  ImGui::End();
  ImGui::PopStyleVar();

  if (refreshRequested && onRefresh_) onRefresh_();
}
