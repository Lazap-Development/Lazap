#include "ui/panels/settings.h"

#include <imgui.h>

#include "ui/themes/themes.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

using namespace ui;

void SettingsPanel::init() {
  ImageManager::loadSVG(b::embed<"assets/svg/config.svg">(), "config:white",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/config.svg">(), "config:dark",
                        0x000000FF);
  ImageManager::loadSVG(b::embed<"assets/svg/account.svg">(), "account:white",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/account.svg">(), "account:dark",
                        0x000000FF);
  ImageManager::loadSVG(b::embed<"assets/svg/monitor.svg">(), "monitor",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/appearance.svg">(), "appearance",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/link.svg">(), "link", 0xFFFFFFFF);
}

void SettingsPanel::render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(35, 50));
  ImGui::Begin(name_.c_str(), nullptr, false);
  ImVec2 scale = Themes::getScale(1675, 880);

  ImGui::SameLine(0, 10 * scale.x);
  if (view_ == SettingsView::LauncherConfig) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    drawlist->AddRectFilled(
        ImVec2(p.x, p.y), ImVec2(p.x + (143 * scale.x), p.y + (25 * scale.y)),
        IM_COL32(242, 244, 238, 255), 3.0f,
        ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);
    ImGui::PushFont(FontManager::getFont("Settings:Button"));
    addIcon("config:dark", scale);
  } else {
    ImGui::PushFont(FontManager::getFont("Settings:Button"));
    addIcon("config:white", scale);
  }
  bool lcbtn = ImGui::Button("Launcher Config");
  if (view_ == SettingsView::LauncherConfig) {
    ImGui::PopStyleColor();
  }
  ImGui::PopFont();
  if (lcbtn) {
    view_ = SettingsView::LauncherConfig;
  }

  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (22 * scale.x));
  if (view_ == SettingsView::AccountSettings) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    drawlist->AddRectFilled(
        ImVec2(p.x, p.y), ImVec2(p.x + (143 * scale.x), p.y + (25 * scale.y)),
        IM_COL32(242, 244, 238, 255), 3.0f,
        ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);
    ImGui::PushFont(FontManager::getFont("Settings:Button"));
    addIcon("account:dark", scale);
  } else {
    ImGui::PushFont(FontManager::getFont("Settings:Button"));
    addIcon("account:white", scale);
  }
  bool asbtn = ImGui::Button("Account Settings");
  if (view_ == SettingsView::AccountSettings) {
    ImGui::PopStyleColor();
  }
  ImGui::PopFont();
  if (asbtn) {
    view_ = SettingsView::AccountSettings;
  }

  ImDrawList* drawlist = ImGui::GetWindowDrawList();
  drawlist->AddLine(ImVec2(ImGui::GetCursorScreenPos().x - 10 * scale.x,
                           ImGui::GetCursorScreenPos().y + 3 * scale.y),
                    ImVec2(ImGui::GetCursorScreenPos().x + (1605 * scale.x),
                           ImGui::GetCursorScreenPos().y + (5 * scale.y)),
                    IM_COL32(242, 244, 238, 255), 2.0f);

  ImGui::Dummy(ImVec2(0, 45 * scale.y));
  switch (view_) {
    case SettingsView::LauncherConfig:

      ImGui::PushFont(FontManager::getFont("Title"));
      ImGui::Image(ImageManager::get("monitor"),
                   ImVec2(27 * scale.x, 27 * scale.x));
      ImGui::SameLine();
      ImGui::Text("General");
      ImGui::PopFont();
      ImGui::Dummy(ImVec2(0, 23 * scale.y));

      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::Dummy(ImVec2(35 * scale.x, 0));
      ImGui::SameLine();
      ImGui::PushID("general_settings");
      ImGui::BeginGroup();
      ImGui::Text("Minimize to tray on quit");
      // printf("%f", ImGui::GetContentRegionAvail().x +
      // ImGui::GetCursorPosX());

      ImGui::Text("Launch at startup");
      ImGui::Text("Automatically check for updates");
      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();

      ImGui::Dummy(ImVec2(0, 47 * scale.y));
      ImGui::PushFont(FontManager::getFont("Title"));
      ImGui::Image(ImageManager::get("appearance"),
                   ImVec2(27 * scale.x, 27 * scale.x));
      ImGui::SameLine();
      ImGui::Text("Appearance");
      ImGui::PopFont();
      ImGui::Dummy(ImVec2(0, 23 * scale.y));

      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::Dummy(ImVec2(35, 0));
      ImGui::SameLine();
      ImGui::PushID("appearance_settings");
      ImGui::BeginGroup();
      ImGui::Text("Accent color");
      ImGui::Text("Background Image");
      ImGui::Text("Background Image opacity");
      ImGui::Text("Show launcher icons");
      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();

      ImGui::Dummy(ImVec2(0, 47 * scale.y));
      ImGui::PushFont(FontManager::getFont("Title"));
      ImGui::Image(ImageManager::get("link"),
                   ImVec2(27 * scale.x, 27 * scale.x));
      ImGui::SameLine();
      ImGui::Text("Integrations");
      ImGui::PopFont();

      ImGui::Dummy(ImVec2(0, 23 * scale.y));
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::Dummy(ImVec2(35, 0));
      ImGui::SameLine();
      ImGui::PushID("integration_settings");
      ImGui::BeginGroup();
      ImGui::Text("Discord Rich Presence(RPC)");
      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();
      break;
    case SettingsView::AccountSettings:
      ImGui::PushFont(FontManager::getFont("Title"));
      ImGui::Image(ImageManager::get("appearance"),
                   ImVec2(27 * scale.x, 27 * scale.x));
      ImGui::Text("Account Settings");
      ImGui::PopFont();
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::Dummy(ImVec2(0, 10));
      ImGui::Text("Username: ");
      // ImGui::InputText("##username", &username_buffer_);
      ImGui::PopFont();
      break;
    default:
      view_ = SettingsView::LauncherConfig;
      break;
  }

  ImGui::End();
  ImGui::PopStyleVar();
}

void addIcon(const std::string& icon, ImVec2 scale) {
  ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (11 * scale.x),
                             ImGui::GetCursorPosY() + (5 * scale.y)));
  ImGui::Image(ImageManager::get(icon), ImVec2(13 * scale.x, 13 * scale.x));
  ImGui::SameLine();
  ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (4 * scale.x),
                             ImGui::GetCursorPosY() + (5 * scale.y)));
}