#include "ui/cmps/addgame_dialog.h"

#include <imgui.h>

#include <cstring>

#include "clients/custom_games.h"
#include "tinyfiledialogs.h"
#include "ui/themes/themes.h"
#include "utils/font_manager.h"

using namespace ui;

AddGameDialog::AddGameDialog(Storage* storage)
    : Panel("Add New Game", storage), size_(500.0f, 600.0f) {
  bannerTexture_ = ImageManager::loadPNG(
      b::embed<"assets/img/default-game-banner.png">(), "default-game-banner");

  bannerPath_[0] = '\0';
}

void AddGameDialog::open() { isOpen_ = true; }

void AddGameDialog::close() {
  isOpen_ = false;

  gameName_[0] = '\0';
  filePath_[0] = '\0';
  bannerPath_[0] = '\0';

  bannerTexture_ = ImageManager::loadPNG(
      b::embed<"assets/img/default-game-banner.png">(), "default-game-banner");

  ImGui::CloseCurrentPopup();
}

void AddGameDialog::render() {
  if (isOpen_) {
    ImGui::OpenPopup("Add New Game");
    isOpen_ = false;
  }

  ImGui::SetNextWindowSize(ImVec2(size_), ImGuiCond_FirstUseEver);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 40));

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

  bool open = true;
  if (ImGui::BeginPopupModal("Add New Game", &open, flags)) {
    ImGui::PushFont(FontManager::getFont("Dialog:Paragraph"));

    ImGui::PushFont(FontManager::getFont("Dialog:Title"));
    const char* title = "Add New Game";
    float windowWidth = ImGui::GetWindowWidth();
    float textWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", title);
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, 50));

    ImGui::Text("Game Name");
    ImGui::Dummy(ImVec2(0, 5));
    ImGui::SetNextItemWidth(-1);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
    ImGui::InputText("##gamename", gameName_, IM_ARRAYSIZE(gameName_));
    ImGui::PopStyleVar();
    Themes::drawInputBorder();

    ImGui::Dummy(ImVec2(0, 15));

    ImGui::Text("Enter Executable Path");
    ImGui::Dummy(ImVec2(0, 5));
    ImGui::SetNextItemWidth(-170);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
    ImGui::InputText("##filepath", filePath_, IM_ARRAYSIZE(filePath_));
    ImGui::PopStyleVar();
    Themes::drawInputBorder();
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         ImGui::GetContentRegionAvail().x - 115);
    if (ImGui::Button("Select File", ImVec2(150, 0))) {
      const char* path = tinyfd_openFileDialog("Select Game Executable", "", 0,
                                               nullptr, nullptr, 0);
      if (path) {
        std::strncpy(filePath_, path, IM_ARRAYSIZE(filePath_));
        filePath_[IM_ARRAYSIZE(filePath_) - 1] = '\0';
      }
    }
    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    ImGui::Dummy(ImVec2(0, 15));
    ImGui::Text("Choose Game Banner (Optional)");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         ImGui::GetContentRegionAvail().x - 115);
    if (ImGui::Button("Select File##banner", ImVec2(150, 0))) {
      const char* filters[] = {"*.png"};
      const char* path = tinyfd_openFileDialog("Select Banner Image", "", 1,
                                               filters, "Image files", 0);
      if (path) {
        bannerTexture_ = ImageManager::loadPNG(path);
        std::strncpy(bannerPath_, path, IM_ARRAYSIZE(bannerPath_));
        bannerPath_[IM_ARRAYSIZE(bannerPath_) - 1] = '\0';
      }
    }
    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    ImGui::Dummy(ImVec2(0, 10));

    if (bannerTexture_.id) {
      float contentWidth = ImGui::GetContentRegionAvail().x;
      float aspect = 350.0f / 820.0f;
      float displayWidth = contentWidth * 0.9f;
      float displayHeight = displayWidth * aspect;
      float offsetX = (contentWidth - displayWidth) * 0.5f;
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
      ImGui::Image(bannerTexture_.id, ImVec2(displayWidth, displayHeight));
    }

    ImGui::PopFont();

    float btnHeight = 35.0f;
    float bottomPadding = 30.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - btnHeight - bottomPadding);
    float btnWidth = 225.0f;
    float spacing = 10.0f;
    float totalWidth = btnWidth * 2 + spacing;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

    ImGui::PushFont(FontManager::getFont("Dialog:Button"));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    if (ImGui::Button("Cancel", ImVec2(btnWidth, 35))) close();
    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0, spacing);

    ImVec4 accentColor =
        ImGui::ColorConvertU32ToFloat4(Themes::ACCENT_COLOR_IMGUI);
    ImGui::PushStyleColor(ImGuiCol_Button, accentColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(accentColor.x * 1.15f, accentColor.y * 1.15f,
                                 accentColor.z * 1.15f, accentColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(accentColor.x * 0.85f, accentColor.y * 0.85f,
                                 accentColor.z * 0.85f, accentColor.w));
    if (ImGui::Button("Add Game", ImVec2(btnWidth, 35))) {
      if (std::strlen(gameName_) > 0 && std::strlen(filePath_) > 0) {
        CustomGames customGames(*storage_);
        if (customGames.addCustomGame(filePath_, gameName_, bannerPath_))
          requestRefresh();
      }
      close();
    }
    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::PopStyleColor(3);

    ImGui::PopFont();

    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    ImGui::GetForegroundDrawList()->AddRect(
        pos, ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(162, 162, 162, 255), 5.0f, 0, 2.0f);

    ImGui::EndPopup();
  }

  ImGui::PopStyleVar();
}
