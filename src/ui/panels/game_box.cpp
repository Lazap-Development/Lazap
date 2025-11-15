// clang-format off
#include "ui/panels/game_box.h"
// clang-format on
#include <imgui.h>

#include "utils/fnv1a.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"
#include "utils/launch_manager.h"
using namespace ui;

void GameBox::render() {
  ImGui::PushID(name_.c_str());
  ImGui::BeginGroup();

  ImGui::Image(ImageManager::get("banner"), ImVec2(210, 233.1));

  LaunchManager lm = LaunchManager(game_);
  if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) lm.launch();
  if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
    storage_->updateTOML([this](toml::table& config) {
      if (config.contains("games")) {
        auto gamesTable = config["games"].as_table();
        if (gamesTable->contains(std::to_string(fnv1a::hash(
                game_.name.c_str(), std::strlen(game_.name.c_str()))))) {
          auto gameTable =
              gamesTable
                  ->at(std::to_string(fnv1a::hash(
                      game_.name.c_str(), std::strlen(game_.name.c_str()))))
                  .as_table();
          gameTable->insert_or_assign("favourite", toml::value(true));
        }
      }
    });
    if (lm.isRunning()) {
      lm.kill();
    }
  }

  ImVec2 min = ImGui::GetItemRectMin();
  ImVec2 max = ImGui::GetItemRectMax();
  bool hover = ImGui::IsItemHovered();
  if (hover) {
    auto* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(min, max, IM_COL32(73, 34, 88, 40));
    ImVec2 center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
    draw->AddImage(ImageManager::get("play"), center,
                   ImVec2(center.x + 32.0f, center.y + 32.0f), ImVec2(0, 0),
                   ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
  }

  ImGui::PushFont(FontManager::getFont("GameBox:Title"));
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
  ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 154.0f);
  ImGui::TextWrapped(name_.c_str());
  ImGui::PopTextWrapPos();
  ImGui::PopFont();

  float iconSize = 16.0f;
  float spacing = 6.0f;
  float totalIconsWidth = (iconSize * 2) + (spacing * 2);
  float cursorX = ImGui::GetCursorPosX() + 210.0f - totalIconsWidth;

  ImGui::SameLine();
  ImGui::SetCursorPosX(cursorX);
  ImGui::Image(ImageManager::get("heart2"), ImVec2(iconSize, iconSize));
  ImGui::SameLine(0, spacing);
  ImGui::Image(ImageManager::get("recent"), ImVec2(iconSize, iconSize));

  ImGui::EndGroup();
  ImGui::PopID();
}
