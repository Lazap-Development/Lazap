// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
// clang-format on

#include "ui/panels/left_panel.h"

#include "imgui.h"
#include "utils/image_manager.h"

using namespace ui;

void LeftPanel::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/home.svg">(), "home", 0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/library.svg">(), "library",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart2",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/github.svg">(), "github",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/settings.svg">(), "settings",
                        0xFFFFFFFF);
}

void LeftPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoTitleBar);
  ImGui::Dummy(ImVec2(0, 20));

  // ImGui::PushFont(FontManager::getFont("Left:Button"));
  bool home =
      ImGui::ImageButton("Home", ImageManager::get("home"), ImVec2(24, 24));
  bool favs = ImGui::ImageButton("Favorites", ImageManager::get("heart2"),
                                 ImVec2(24, 24));
  bool library = ImGui::ImageButton("Library", ImageManager::get("library"),
                                    ImVec2(24, 24));

  ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - 150));

  bool github =
      ImGui::ImageButton("GitHub", ImageManager::get("github"), ImVec2(24, 24));
  bool settings = ImGui::ImageButton("Settings", ImageManager::get("settings"),
                                     ImVec2(24, 24));
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
