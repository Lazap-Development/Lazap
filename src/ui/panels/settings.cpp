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
                        0x000000);
  ImageManager::loadSVG(b::embed<"assets/svg/account.svg">(), "account:white",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/account.svg">(), "account:dark",
                        0x000000);
  ImageManager::loadSVG(b::embed<"assets/svg/monitor.svg">(), "monitor",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/appearance.svg">(), "appearance",
                        0xFFFFFFFF);
  ImageManager::loadSVG(b::embed<"assets/svg/link.svg">(), "link", 0xFFFFFFFF);
}

void SettingsPanel::render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(35, 50));
  ImGui::Begin(name_.c_str(), nullptr, false);

  scale_ = getScale();

  // Menu buttons
  ImGui::SameLine();
  addMenuButton("Launcher Config", ImVec2(143 * scale_.x, 32 * scale_.y),
                view_ == SettingsView::LauncherConfig);

  ImGui::SameLine();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (22 * scale_.x));
  addMenuButton("Account Settings", ImVec2(143 * scale_.x, 30 * scale_.y),
                view_ == SettingsView::AccountSettings);

  // Separator line
  ImDrawList* drawlist = ImGui::GetWindowDrawList();
  drawlist->AddLine(ImVec2(ImGui::GetCursorScreenPos().x - 10 * scale_.x,
                           ImGui::GetCursorScreenPos().y + 3 * scale_.y),
                    ImVec2(ImGui::GetCursorScreenPos().x + (1605 * scale_.x),
                           ImGui::GetCursorScreenPos().y + (5 * scale_.y)),
                    IM_COL32(242, 244, 238, 255), 2.0f);

  // Settings
  ImGui::Dummy(ImVec2(0, 45 * scale_.y));
  switch (view_) {
    case SettingsView::LauncherConfig:
      addSection("General", "monitor");
      ImGui::Dummy(ImVec2(35 * scale_.x, 23 * scale_.y));
      ImGui::SameLine();
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::PushID("general_settings");
      ImGui::BeginGroup();
      addOption("Minimize to tray on quit", InputType::Toggle);
      addOption("Launch at startup", InputType::Toggle);
      addOption("Automatically check for updates", InputType::Toggle);
      ImGui::EndGroup();
      ImGui::PopID();
      ImGui::PopFont();

      ImGui::Dummy(ImVec2(0, 47 * scale_.y));
      addSection("Appearance", "appearance");
      // ImGui::PushFont(FontManager::getFont("Title"));
      // ImGui::Image(ImageManager::get("appearance"),
      //              ImVec2(27 * scale_.x, 27 * scale_.x));
      // ImGui::SameLine();
      // ImGui::Text("Appearance");
      // ImGui::PopFont();
      ImGui::Dummy(ImVec2(0, 23 * scale_.y));

      ImGui::Dummy(ImVec2(35, 0));
      ImGui::SameLine();
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::PushID("appearance_settings");
      ImGui::BeginGroup();
      addOption("Accent color", InputType::ColorPicker);
      addOption("Background Image", InputType::ImagePicker);
      addOption("Background Image opacity", InputType::IntTextbox);
      addOption("Show launcher icons", InputType::Toggle);
      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();

      ImGui::Dummy(ImVec2(0, 47 * scale_.y));
      addSection("Integrations", "link");
      // ImGui::PushFont(FontManager::getFont("Title"));
      // ImGui::Image(ImageManager::get("link"),
      //              ImVec2(27 * scale_.x, 27 * scale_.x));
      // ImGui::SameLine();
      // ImGui::Text("Integrations");
      // ImGui::PopFont();
      ImGui::Dummy(ImVec2(0, 23 * scale_.y));

      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::Dummy(ImVec2(35, 0));
      ImGui::SameLine();
      ImGui::PushID("integration_settings");
      ImGui::BeginGroup();
      addOption("Discord Rich Presence(RPC)", InputType::Toggle);
      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();
      break;
    case SettingsView::AccountSettings:
      ImGui::PushFont(FontManager::getFont("Title"));
      ImGui::Image(ImageManager::get("appearance"),
                   ImVec2(27 * scale_.x, 27 * scale_.x));
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

bool SettingsPanel::addMenuButton(const std::string& label, ImVec2 size,
                                  bool active = false) {
  if (active) {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(8, 8, 8, 255));
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    drawlist->AddRectFilled(
        ImVec2(p.x, p.y), ImVec2(p.x + (size.x), p.y + (size.y)),
        IM_COL32(242, 244, 238, 255), 3.0f,
        ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);
    addIcon(view_ == SettingsView::LauncherConfig ? "config:dark"
                                                  : "account:dark");
  } else {
    addIcon(view_ == SettingsView::AccountSettings ? "config:white"
                                                   : "account:white");
  }
  ImGui::PushFont(FontManager::getFont("Settings:Button"));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10 * scale_.x, 0));
  bool btn = ImGui::Button(label.c_str());
  ImGui::PopStyleVar();
  ImGui::PopFont();
  if (active) {
    ImGui::PopStyleColor();
  }
  if (btn && !active) {
    view_ =
        view_ == SettingsView::LauncherConfig    ? SettingsView::AccountSettings
        : view_ == SettingsView::AccountSettings ? SettingsView::LauncherConfig
                                                 : SettingsView::LauncherConfig;
  }
  return btn;
}

void SettingsPanel::addIcon(const std::string& icon) {
  ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (11 * scale_.x),
                             ImGui::GetCursorPosY() + (5 * scale_.y)));
  ImGui::Image(ImageManager::get(icon), ImVec2(13 * scale_.x, 13 * scale_.x));
  ImGui::SameLine();
  ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (4 * scale_.x),
                             ImGui::GetCursorPosY() + (5 * scale_.y)));
}

void SettingsPanel::addSection(const std::string& title,
                               const std::string& icon) {
  ImGui::PushFont(FontManager::getFont("Title"));
  ImGui::Dummy(ImVec2(8 * scale_.x, 0));
  ImGui::Image(ImageManager::get(icon), ImVec2(27 * scale_.x, 27 * scale_.x));
  ImGui::SameLine();
  ImGui::Text(title.c_str());
  ImGui::PopFont();
}

void SettingsPanel::addOption(const std::string& label, InputType input) {
  ImGui::BeginChild(label.c_str(), ImVec2(740 * scale_.x, 45 * scale_.y), false,
                    ImGuiWindowFlags_NoDecoration);
  ImGui::Text(label.c_str());
  ImGui::SameLine(
      0, ImGui::GetContentRegionAvail().x -
             (ImGui::GetWindowSize().x - 740 * scale_.x - 40 * scale_.x));

  std::string id = (label).c_str();
  std::replace(id.begin(), id.end(), ' ', '-');
  switch (input) {
    case InputType::Toggle:
      ImGui::Checkbox(id.c_str(), nullptr);
      break;
    case InputType::ColorPicker:
      static float c[3] = {0, 0, 0};
      ImGui::ColorPicker3(id.c_str(), c);
      break;
    case InputType::ImagePicker:
      if (ImGui::Button(id.c_str(), ImVec2(110, 30))) {
        // File dialog handling
      }
      break;
    case InputType::IntTextbox:
      ImGui::SliderFloat(id.c_str(), nullptr, 0.0f, 100.0f);
      break;
    case InputType::StringTextbox:
      ImGui::InputText(id.c_str(), "Username", 32);
      break;
  }
  ImGui::EndChild();
}
