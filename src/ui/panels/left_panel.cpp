// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
// clang-format on

#include "ui/panels/left_panel.h"

#include "imgui.h"
#include "ui/themes/themes.h"
#include "utils/image_manager.h"

using namespace ui;

void openURL(const std::string& url) {
#ifdef _WIN32
  std::string command = "start " + url;
#elif __linux__
  std::string command = "xdg-open " + url;
#endif
  std::system(command.c_str());
}

void LeftPanel::init() {
  ImageManager::loadPNG(b::embed<"assets/icons/lazap/lazap.png">(), "lazap");
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

  if (scale_.x == 0) {
    scale_ = Themes::getScale(80, 1000);
  }
  // Lazap Icon
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(15.0f * scale_.x, 20.0f * scale_.y));
  ImGui::ImageButton("##lazap", ImageManager::get("lazap"),
                     ImVec2(50 * scale_.x, 50 * scale_.x));
  ImGui::PopStyleVar();

  ImGui::Dummy(ImVec2(0, ImGui::GetWindowSize().y * 0.07219f * scale_.y));
  // Middle Icons
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(25.0f * scale_.x, 0.0f));
  bool home = ImGui::ImageButton("Home", ImageManager::get("home"),
                                 ImVec2(24 * scale_.x, 24 * scale_.x));
  ImGui::Dummy(ImVec2(0, 41.71f * scale_.y));
  bool favs = ImGui::ImageButton("Favorites", ImageManager::get("heart2"),
                                 ImVec2(24 * scale_.x, 24 * scale_.x));
  ImGui::Dummy(ImVec2(0, 41.71f * scale_.y));
  bool library = ImGui::ImageButton("Library", ImageManager::get("library"),
                                    ImVec2(24 * scale_.x, 24 * scale_.x));
  // TODO: Add hover effect and active indicator

  ImGui::Dummy(
      ImVec2(0, ImGui::GetContentRegionAvail().y - (103.25f * scale_.y)));

  // Bottom Icons
  bool github = ImGui::ImageButton("GitHub", ImageManager::get("github"),
                                   ImVec2(24 * scale_.x, 24 * scale_.x));
  ImGui::Dummy(ImVec2(0, 30.25f * scale_.y));
  bool settings = ImGui::ImageButton("Settings", ImageManager::get("settings"),
                                     ImVec2(24 * scale_.x, 24 * scale_.x));
  ImGui::Dummy(ImVec2(0, 25.0f * scale_.y));
  ImGui::PopStyleVar();

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
    std::string url = "https://github.com/Lazap-Development/Lazap";
    openURL(url);
  }
  if (settings == true && view_->view != ViewType::Settings) {
    view_->Settings();
  }
  ImGui::Separator();
  ImGui::End();
}
