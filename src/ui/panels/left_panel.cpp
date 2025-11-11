#include "ui/panels/left_panel.h"

#include "imgui.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void LeftPanel::init() {
  ImageManager::LoadSVG("src/assets/svg/home.svg", "home", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/library.svg", "library", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/heart2.svg", "heart2", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/github.svg", "github", 0xFFFFFFFF);
  ImageManager::LoadSVG("src/assets/svg/settings.svg", "settings", 0xFFFFFFFF);
}

void LeftPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);

  int y = ImGui::GetMainViewport()->Size.y;
  ImGui::Dummy(ImVec2(0, 60));
  // ImGui::PushFont(FontManager::GetFont("Left:Button"));
  bool home =
      ImGui::ImageButton("Home", ImageManager::GetIcon("home"), ImVec2(24, 24));
  bool favs = ImGui::ImageButton("Favorites", ImageManager::GetIcon("heart2"),
                                 ImVec2(24, 24));
  bool library = ImGui::ImageButton("Library", ImageManager::GetIcon("library"),
                                    ImVec2(24, 24));

  ImGui::Dummy(ImVec2(0, y - 138 - ImGui::GetCursorPosY()));
  bool github = ImGui::ImageButton("GitHub", ImageManager::GetIcon("github"),
                                   ImVec2(24, 24));
  bool settings = ImGui::ImageButton(
      "Settings", ImageManager::GetIcon("settings"), ImVec2(24, 24));
  // ImGui::PopFont();

  if (home == true && view_->view != ViewType::MainMenu) {
    view_->MainMenu();
  }
  if (favs == true && view_->view != ViewType::Favorites) {
    view_->Favorites();
  }
  if (library == true && view_->view != ViewType::Library) {
    view_->Library();
  }

  if (github == true) {
    std::string url = "";
  }
  if (settings == true) {
  }
  ImGui::Separator();
  ImGui::End();
}
