#include "ui/cmps/game_box.h"

#include <imgui.h>

#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"
#include "utils/launch_manager.h"

using namespace ui;

GameBox::GameBox(const Game& game, Storage* storage)
    : Panel(game.name, storage), game_(game) {
  bannerTexture_ = game_.bannerUrl.empty()
                       ? ImageManager::loadPNG(
                             b::embed<"assets/img/default-game-banner.png">(),
                             "default-game-banner")
                       : ImageManager::loadPNG(game_.bannerUrl);
}

void GameBox::render() {
  ImGui::PushID(name_.c_str());
  ImGui::BeginGroup();

  scale_ = getScale();

  const ImVec2 displaySize(210.0f * scale_.x, 233.1f * scale_.y);
  const float cornerRadius =
      8.0f * sqrt((pow(scale_.x, 2) + pow(scale_.y, 2)) * 0.5f);
  const float topOffsetPixels = 10.0f * scale_.y;
  const float padding = 10.0f;
  const float iconSize = 20.0f * scale_.x;
  const float iconSpacing = 6.0f * scale_.x;
  const float playIconSize = 32.0f * scale_.x;

  ImVec2 uv0(0.0f, 0.0f);
  ImVec2 uv1(1.0f, 1.0f);

  const ImVec2 textureSize(bannerTexture_.width, bannerTexture_.height);
  if (textureSize.x > 0 && textureSize.y > 0) {
    const float texAspect = textureSize.x / textureSize.y;
    const float dispAspect = displaySize.x / displaySize.y;

    if (texAspect > dispAspect) {
      // Texture is wider - crop sides
      const float scale_ = dispAspect / texAspect;
      const float offset = (1.0f - scale_) * 0.5f;
      uv0.x = offset;
      uv1.x = 1.0f - offset;
    } else {
      // Texture is taller - crop top/bottom with offset
      const float scale_ = texAspect / dispAspect;
      uv0.y = topOffsetPixels / textureSize.y;
      uv1.y = uv0.y + scale_;
    }
  }

  ImDrawList* drawList = ImGui::GetWindowDrawList();
  const ImVec2 pos = ImGui::GetCursorScreenPos();
  const ImVec2 end(pos.x + displaySize.x, pos.y + displaySize.y);

  drawList->AddImageRounded(bannerTexture_.id, pos, end, uv0, uv1,
                            IM_COL32_WHITE, cornerRadius,
                            ImDrawFlags_RoundCornersAll);

  ImGui::Dummy(displaySize);

  const bool isHovered = ImGui::IsItemHovered();
  const ImVec2 itemMin = ImGui::GetItemRectMin();
  const ImVec2 itemMax = ImGui::GetItemRectMax();

  LaunchManager lm(game_);

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
    lm.launch();

    storage_->updateTOML([this](toml::table& config) {
      auto games = config["games"].as_table();
      if (!games) return;

      const std::string key = std::to_string(
          fnv1a::hash(game_.name.c_str(), std::strlen(game_.name.c_str())));

      if (!games->contains(key)) return;

      auto* table = games->at(key).as_table();
      if (!table) return;

      // ISO-8601 timestamp
      char buf[64];
      std::time_t t = std::time(nullptr);
      std::tm tm{};
#ifdef _WIN32
      localtime_s(&tm, &t);
#else
      localtime_r(&t, &tm);
#endif
      std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);

      table->insert_or_assign("last_launch", std::string(buf));
    });
  }

  if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
    storage_->updateTOML([this](toml::table& config) {
      auto games = config["games"].as_table();
      if (!games) return;

      const std::string key = std::to_string(
          fnv1a::hash(game_.name.c_str(), std::strlen(game_.name.c_str())));

      if (games->contains(key)) {
        auto* table = games->at(key).as_table();
        bool current = table->get("favourite")->value_or(false);
        table->insert_or_assign("favourite", !current);
      }
    });

    if (lm.isRunning()) {
      lm.kill();
    }

    requestRefresh();
  }

  if (isHovered) {
    drawList->AddRectFilled(itemMin, itemMax, IM_COL32(73, 34, 88, 40),
                            cornerRadius);

    const ImVec2 center((itemMin.x + itemMax.x) * 0.5f,
                        (itemMin.y + itemMax.y) * 0.5f);

    drawList->AddImage(
        ImageManager::get("play"),
        ImVec2(center.x - playIconSize * 0.5f, center.y - playIconSize * 0.5f),
        ImVec2(center.x + playIconSize * 0.5f, center.y + playIconSize * 0.5f),
        ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE);

    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  ImGui::PushFont(FontManager::getFont("GameBox:Title"));
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding * scale_.x);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * scale_.y);

  const float totalIconsWidth = iconSize * 2 + iconSpacing;
  const float availableTextWidth = displaySize.x - (padding * scale_.x * 2) -
                                   totalIconsWidth - (iconSpacing * 2);

  std::string displayName = name_;

  const float lineHeight = ImGui::GetTextLineHeightWithSpacing();
  ImVec2 textSize = ImGui::CalcTextSize(displayName.c_str(), nullptr, false,
                                        availableTextWidth);

  while (textSize.y > lineHeight * 2.0f && displayName.length() > 3) {
    displayName = displayName.substr(0, displayName.length() - 1);
    std::string test = displayName + "...";
    textSize =
        ImGui::CalcTextSize(test.c_str(), nullptr, false, availableTextWidth);
  }

  if (displayName.length() < name_.length()) displayName += "...";

  ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + availableTextWidth);
  ImGui::TextWrapped("%s", displayName.c_str());
  ImGui::PopTextWrapPos();
  ImGui::PopFont();

  const float iconStartX = ImGui::GetCursorPosX() + displaySize.x -
                           totalIconsWidth - (padding * scale_.x);

  auto toml = storage_->loadTOML();
  auto* gamesTable = toml["games"].as_table();
  bool isFavourite = false;

  if (gamesTable) {
    const std::string key = std::to_string(
        fnv1a::hash(game_.name.c_str(), std::strlen(game_.name.c_str())));

    if (gamesTable->contains(key)) {
      auto* table = gamesTable->at(key).as_table();
      if (table) {
        isFavourite = table->get("favourite")->value_or(false);
      }
    }
  }

  ImGui::SameLine();
  ImGui::SetCursorPosX(iconStartX);
  ImGui::Image(ImageManager::get(isFavourite ? "heart2-solid" : "heart2"),
               ImVec2(iconSize, iconSize));

  ImGui::SameLine(0, iconSpacing);
  ImGui::Image(ImageManager::get("recent"), ImVec2(iconSize, iconSize));

  ImGui::EndGroup();
  ImGui::PopID();
}
