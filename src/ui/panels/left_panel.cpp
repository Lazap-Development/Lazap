#include "ui/panels/left_panel.h"

#include <imgui.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ui/theme.h"
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
  ImageManager::loadSVG(b::embed<"assets/svg/home.svg">(), "home-black",
                        0xFF000000);
  ImageManager::loadSVG(b::embed<"assets/svg/library.svg">(), "library",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/library.svg">(), "library-black",
                        0xFF000000);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart-white",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/heart2.svg">(), "heart-black",
                        0xFF000000);
  ImageManager::loadSVG(b::embed<"assets/svg/github.svg">(), "github",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/settings.svg">(), "settings",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/settings.svg">(), "settings-black",
                        0xFF000000);
}

void LeftPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollbar);

  scale_ = getScale();

  auto drawIconButton = [&](const char* id, ImTextureID texture,
                            ImTextureID activeTexture, const ImVec2& size,
                            bool isActive) -> bool {
    ImVec2 cursorPos = ImGui::GetCursorPos();

    if (isActive) {
      ImDrawList* drawList = ImGui::GetWindowDrawList();
      ImVec2 windowPos = ImGui::GetWindowPos();
      ImVec2 rectMin = ImVec2(windowPos.x + cursorPos.x - 38.0f * scale_.x,
                              windowPos.y + cursorPos.y - 12.5f * scale_.y);
      ImVec2 rectMax =
          ImVec2(windowPos.x + cursorPos.x + size.x + 38.0f * scale_.x,
                 windowPos.y + cursorPos.y + size.y + 12.5f * scale_.y);

      drawList->AddRectFilled(
          rectMin, rectMax, Themes::ACCENT_COLOR_IMGUI,
          12.0f * sqrt((pow(scale_.x, 2) + pow(scale_.y, 2)) * 0.5f));
    }

    bool clicked =
        ImGui::ImageButton(id, isActive ? activeTexture : texture, size);

    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    return clicked;
  };

  // Top icon
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(15.0f * scale_.x, 20.0f * scale_.y));
  ImGui::ImageButton("##lazap", ImageManager::get("lazap"),
                     ImVec2(50 * scale_.x, 50 * scale_.x));
  ImGui::PopStyleVar();

  ImGui::Dummy(ImVec2(0, ImGui::GetWindowSize().y * 0.07f * scale_.y));

  // Middle icons
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                      ImVec2(25.0f * scale_.x, 0.0f));

  if (drawIconButton("Home", ImageManager::get("home"),
                     ImageManager::get("home-black"),
                     ImVec2(30 * scale_.x, 30 * scale_.x),
                     view_->view == ViewType::MainMenu))
    if (view_->view != ViewType::MainMenu) view_->MainMenu();

  ImGui::Dummy(ImVec2(0, 42.0f * scale_.y));

  if (drawIconButton("Favorites", ImageManager::get("heart-white"),
                     ImageManager::get("heart-black"),
                     ImVec2(30 * scale_.x, 30 * scale_.x),
                     view_->view == ViewType::Favorites))
    if (view_->view != ViewType::Favorites) view_->Favorites();

  ImGui::Dummy(ImVec2(0, 42.0f * scale_.y));

  if (drawIconButton("Library", ImageManager::get("library"),
                     ImageManager::get("library-black"),
                     ImVec2(30 * scale_.x, 30 * scale_.x),
                     view_->view == ViewType::Library))
    if (view_->view != ViewType::Library) view_->Library();

  ImGui::Dummy(
      ImVec2(0, ImGui::GetContentRegionAvail().y - (125.0f * scale_.y)));

  // Bottom icons
  if (drawIconButton("GitHub", ImageManager::get("github"),
                     ImageManager::get("github"),
                     ImVec2(30 * scale_.x, 30 * scale_.x), false))
    openURL("https://github.com/Lazap-Development/Lazap");

  ImGui::Dummy(ImVec2(0, 30.0f * scale_.y));

  if (drawIconButton("Settings", ImageManager::get("settings"),
                     ImageManager::get("settings-black"),
                     ImVec2(30 * scale_.x, 30 * scale_.x),
                     view_->view == ViewType::Settings))
    if (view_->view != ViewType::Settings) view_->Settings();

  ImGui::PopStyleVar();

  ImGui::End();
}
