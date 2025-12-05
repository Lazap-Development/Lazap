#include "ui/panels/left_panel.h"

#include <imgui.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "ui/themes/themes.h"
#include "utils/image_manager.h"

using namespace ui;

void openURL(const std::string& url) {
#ifdef _WIN32
  ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __linux__
  std::system(("xdg-open " + url).c_str());
#endif
}

void LeftPanel::init() {
  ImageManager::loadPNG(b::embed<"assets/icons/lazap/lazap.png">(), "lazap");
  ImageManager::loadSVG(b::embed<"assets/svg/home.svg">(), "home", 0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/library.svg">(), "library",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/heart.svg">(), "heart",
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

  auto drawIconButton = [&](const char* id, ImTextureID texture,
                            const ImVec2& size) -> bool {
    bool clicked = ImGui::ImageButton(id, texture, size);
    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    return clicked;
  };

  // Top icon
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(15.0f * scale_.x, 20.0f * scale_.y));
  ImGui::ImageButton("##lazap", ImageManager::get("lazap"),
                     ImVec2(50 * scale_.x, 50 * scale_.x));
  ImGui::PopStyleVar();

  ImGui::Dummy(ImVec2(0, ImGui::GetWindowSize().y * 0.07219f * scale_.y));

  // Middle icons
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(25.0f * scale_.x, 0.0f));
  if (drawIconButton("Home", ImageManager::get("home"),
                     ImVec2(28 * scale_.x, 28 * scale_.x)))
    if (view_->view != ViewType::MainMenu) view_->MainMenu();

  ImGui::Dummy(ImVec2(0, 41.71f * scale_.y));

  if (drawIconButton("Favorites", ImageManager::get("heart"),
                     ImVec2(28 * scale_.x, 28 * scale_.x)))
    if (view_->view != ViewType::Favorites) view_->Favorites();

  ImGui::Dummy(ImVec2(0, 41.71f * scale_.y));

  if (drawIconButton("Library", ImageManager::get("library"),
                     ImVec2(28 * scale_.x, 28 * scale_.x)))
    if (view_->view != ViewType::Library) view_->Library();

  ImGui::Dummy(
      ImVec2(0, ImGui::GetContentRegionAvail().y - (115.0f * scale_.y)));

  // Bottom icons
  if (drawIconButton("GitHub", ImageManager::get("github"),
                     ImVec2(28 * scale_.x, 28 * scale_.x)))
    openURL("https://github.com/Lazap-Development/Lazap");

  ImGui::Dummy(ImVec2(0, 28.25f * scale_.y));

  if (drawIconButton("Settings", ImageManager::get("settings"),
                     ImVec2(28 * scale_.x, 28 * scale_.x)))
    if (view_->view != ViewType::Settings) view_->Settings();

  ImGui::Dummy(ImVec2(0, 25.0f * scale_.y));
  ImGui::PopStyleVar();

  ImGui::Separator();
  ImGui::End();
}
