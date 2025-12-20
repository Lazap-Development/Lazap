#include "ui/panels/game_panel.h"

#include <imgui.h>

#include <algorithm>

#include "clients/custom_games.h"
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
  FontManager::loadFont("CustomGame:Plus",
                        b::embed<"assets/fonts/Oxanium-ExtraLight.ttf">(),
                        64.0f);
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

      if (*view_ == ViewType::Library) {
        ImGui::TableNextColumn();
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(210 * scale_.x, 233.1 * scale_.y);
        draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                      IM_COL32(173, 173, 173, 255), 5.25f, 0, 1.0f);
        ImGui::InvisibleButton("hitbox", size);
        if (ImGui::IsItemClicked()) {
          printf("Clicked");
          ImGui::OpenPopup("custom_game_dialog");
        }
        ImGui::PushFont(FontManager::getFont("CustomGame:Plus"));
        ImVec2 text_size = ImGui::CalcTextSize("+");
        ImGui::SetCursorPos(
            ImVec2(pos.x + ((size.x - text_size.x) / 2),
                   pos.y + ((size.y - text_size.y) / 2) - size.y));
        ImGui::Dummy(ImVec2(0, 80 * scale_.y * 2));
        ImGui::Dummy(ImVec2(86 * scale_.x, 0));
        ImGui::SameLine();
        ImGui::Text("+");
        ImGui::PopFont();
        GamePanel::OpenCustomGameDialog();
      }
      ImGui::EndTable();
    }

    ImGui::PopStyleVar();
  }

  ImGui::End();
  ImGui::PopStyleVar();

  if (refreshRequested && onRefresh_) onRefresh_();
}

void GamePanel::OpenCustomGameDialog() {
  if (ImGui::BeginPopupModal(
          "custom_game_dialog", nullptr,
          ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
    char customGameName_[128] = "";
    char location_[512] = "";
    char banner_[512] = "";
    ImGui::Text("Add New Game");
    ImGui::Text("Game Name");
    ImGui::InputText("##game_name", customGameName_,
                     IM_ARRAYSIZE(customGameName_));
    ImGui::Text("Enter file path (.exe)");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110 * scale_.x);
    ImGui::Button("Select file");
    ImGui::InputText("##game_exe", location_, IM_ARRAYSIZE(location_));
    ImGui::Text("Choose game banner (optional)");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110 * scale_.x);
    ImGui::Button("Select file");
    ImGui::ImageButton("##banner", ImageManager::get("new_banner"),
                       ImVec2(460, 200));
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Add Game")) {
      CustomGames custom(*storage_);
      custom.addCustomGame(location_, customGameName_, banner_);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}