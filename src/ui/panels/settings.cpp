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

  loadSettings();
}

void SettingsPanel::loadSettings() {
  auto toml = storage_->loadTOML();
  auto* settingsTable = toml["settings"].as_table();

  if (settingsTable) {
    quitTrayMin_ = settingsTable->get("quit_tray_min")->value_or(false);
    autoStart_ = settingsTable->get("auto_start")->value_or(false);
    checkUpdates_ = settingsTable->get("check_updates")->value_or(true);
    launcherIcons_ = settingsTable->get("launcher_icons")->value_or(true);
    discordRpc_ = settingsTable->get("discord_rpc")->value_or(false);
  }
}

void SettingsPanel::saveSettings() {
  storage_->updateTOML([this](toml::table& config) {
    if (!config.contains("settings")) {
      config.insert("settings", toml::table{});
    }
    auto settingsTable = config["settings"].as_table();

    settingsTable->insert_or_assign("quit_tray_min", quitTrayMin_);
    settingsTable->insert_or_assign("auto_start", autoStart_);
    settingsTable->insert_or_assign("check_updates", checkUpdates_);
    settingsTable->insert_or_assign("launcher_icons", launcherIcons_);
    settingsTable->insert_or_assign("discord_rpc", discordRpc_);
  });
}

void SettingsPanel::render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(35, 50));
  ImGui::Begin(name_.c_str(), nullptr, false);

  scale_ = getScale();

  // Menu buttons
  ImGui::SameLine();
  addMenuButton("Launcher Config", ImVec2(143 * scale_.x, 25 * scale_.y),
                view_ == SettingsView::LauncherConfig);

  // ImGui::SameLine();
  // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (22 * scale_.x));
  // addMenuButton("Account Settings", ImVec2(143 * scale_.x, 25 * scale_.y),
  //               view_ == SettingsView::AccountSettings);

  // Separator line
  ImDrawList* drawlist = ImGui::GetWindowDrawList();
  drawlist->AddLine(
      ImVec2(ImGui::GetCursorScreenPos().x - 10 * scale_.x,
             ImGui::GetCursorScreenPos().y + 3 * scale_.y),
      ImVec2(ImGui::GetCursorScreenPos().x +
                 (ImGui::GetContentRegionAvail().x - 70 * scale_.x),
             ImGui::GetCursorScreenPos().y + (5 * scale_.y)),
      IM_COL32(242, 244, 238, 255), 2.0f);

  // Settings
  ImGui::Dummy(ImVec2(0, 45 * scale_.y));
  switch (view_) {
    case SettingsView::LauncherConfig:
      addSection("General", "monitor");
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::PushID("general_settings");
      ImGui::BeginGroup();

      if (addOption("Minimize to tray on quit", InputType::Toggle,
                    &quitTrayMin_, true)) {
        // saveSettings();
        ImGui::OpenPopup("Coming Soon");
      }
      if (addOption("Launch at startup", InputType::Toggle, &autoStart_,
                    true)) {
        // saveSettings();
        ImGui::OpenPopup("Coming Soon");
      }
      if (addOption("Check for updates", InputType::Toggle, &checkUpdates_,
                    true)) {
        // saveSettings();
        ImGui::OpenPopup("Coming Soon");
      }
      if (ImGui::BeginPopup("Coming Soon", ImGuiWindowFlags_NoMove)) {
        ImGui::Text("This feature is coming soon!");
        ImGui::EndPopup();
      }

      ImGui::EndGroup();
      ImGui::PopID();
      ImGui::PopFont();

      ImGui::Dummy(ImVec2(0, 47 * scale_.y));

      addSection("Appearance", "appearance");
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::PushID("appearance_settings");
      ImGui::BeginGroup();
      addOption("Accent color", InputType::ColorPicker, nullptr);
      addOption("Background Image", InputType::ImagePicker, nullptr);
      addOption("Background Image Opacity", InputType::IntTextbox, nullptr);

      if (addOption("Show launcher icons", InputType::Toggle, &launcherIcons_,
                    true)) {
        // saveSettings();
        ImGui::OpenPopup("Coming Soon");
      }
      if (ImGui::BeginPopup("Coming Soon", ImGuiWindowFlags_NoMove)) {
        ImGui::Text("This feature is coming soon!");
        ImGui::EndPopup();
      }

      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();

      ImGui::Dummy(ImVec2(0, 47 * scale_.y));

      addSection("Integrations", "link");
      ImGui::PushFont(FontManager::getFont("Settings:Setting"));
      ImGui::PushID("integration_settings");
      ImGui::BeginGroup();

      if (addOption("Discord Rich Presence(RPC)", InputType::Toggle,
                    &discordRpc_)) {
        saveSettings();
      }

      ImGui::PopFont();
      ImGui::EndGroup();
      ImGui::PopID();
      break;
    // case SettingsView::AccountSettings:
    //   ImGui::PushFont(FontManager::getFont("Title"));
    //   ImGui::Image(ImageManager::get("appearance"),
    //                ImVec2(27 * scale_.x, 27 * scale_.x));
    //   ImGui::Text("Account Settings");
    //   ImGui::PopFont();
    //   ImGui::PushFont(FontManager::getFont("Settings:Setting"));
    //   ImGui::Dummy(ImVec2(0, 10));
    //   ImGui::Text("Username: ");
    //   // ImGui::InputText("##username", &username_buffer_);
    //   ImGui::PopFont();
    //   break;
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
  ImGui::SameLine(0, 8 * scale_.x);
  ImGui::Text("%s", title.c_str());
  ImGui::PopFont();
  ImGui::Dummy(ImVec2(0, 18.5 * scale_.y));
  ImGui::Dummy(ImVec2(35 * scale_.x, 0));
  ImGui::SameLine();
}

bool SettingsPanel::addOption(const std::string& label, InputType input,
                              bool* value, bool disabled) {
  ImGui::PushID(label.c_str());
  ImGui::BeginChild("row", ImVec2(0, 39 * scale_.y), false);
  ImGui::Dummy(ImVec2(0, 4.5 * scale_.y));

  bool changed = false;

  if (ImGui::BeginTable("option_table", 2, ImGuiTableFlags_SizingFixedFit,
                        ImVec2(590 * scale_.x, 0))) {
    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthFixed,
                            110 * scale_.x);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", label.c_str());
    ImGui::TableSetColumnIndex(1);

    float widthAvailable = ImGui::GetColumnWidth();
    switch (input) {
      case InputType::Toggle: {
        float widthNeeded = 40 * scale_.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             (widthAvailable - widthNeeded));
        if (value) {
          changed = ToggleButton("##toggle", value, disabled);
        }
      } break;

      case InputType::ColorPicker: {
        float widthNeeded = 110 * scale_.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             (widthAvailable - widthNeeded));
        float color[3] = {0, 0, 0};
        ColorBox(label.c_str(), color, ImVec2(110 * scale_.x, 30 * scale_.y));
      } break;

      case InputType::ImagePicker: {
        ImGui::PushFont(FontManager::getFont("Settings:Option"));
        float widthNeeded = 110 * scale_.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             (widthAvailable - widthNeeded));
        if (FilePickerButton(label.c_str(),
                             ImVec2(110 * scale_.x, 30 * scale_.y))) {
          // Open file dialog
        }
        ImGui::PopFont();
      } break;

      case InputType::IntTextbox: {
        float widthNeeded = 92 * scale_.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             (widthAvailable - widthNeeded));
        float n = 0.0f;
        ImGui::PushFont(FontManager::getFont("Settings:Option"));
        NumberBox(label.c_str(), &n, 92 * scale_.x);
        ImGui::PopFont();
      } break;

      case InputType::StringTextbox: {
        float widthNeeded = 490 * scale_.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                             (widthAvailable - widthNeeded));
        char s;
        ImGui::SetNextItemWidth(250 * scale_.x);
        ImGui::InputText("##text", &s, 32);
      } break;
    }

    ImGui::EndTable();
  }

  ImGui::EndChild();
  ImGui::PopID();

  return changed;
}

bool SettingsPanel::ToggleButton(const char* id, bool* v, bool disabled) {
  float height = 20 * scale_.y;
  float width = 40 * scale_.x;
  float radius = 14.67f * scale_.y * 0.5f;

  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImDrawList* draw = ImGui::GetWindowDrawList();

  ImGui::InvisibleButton(id, ImVec2(width, height));
  bool toggled = ImGui::IsItemClicked();

  if (toggled && !disabled) *v = !*v;

  float t = *v ? 1.0f : 0.0f;

  ImU32 col_bg = *v ? IM_COL32(20, 174, 92, 255)     // ON
                    : IM_COL32(162, 162, 162, 105);  // OFF

  ImU32 col_knob = IM_COL32(242, 244, 238, 255);

  // Draw track
  draw->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), col_bg, 50);

  // Draw knob
  float knob_x = pos.x + t * (width - height) + 2.67f * scale_.x;
  draw->AddCircleFilled(
      ImVec2(knob_x + radius, pos.y + 2.67 * scale_.y + radius), radius,
      col_knob);

  return toggled;
}

bool SettingsPanel::ColorBox(const char* id, float color[3], ImVec2 size) {
  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImDrawList* draw = ImGui::GetWindowDrawList();

  ImGui::InvisibleButton(id, size);

  draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                      ImGui::ColorConvertFloat4ToU32(
                          ImVec4(color[0], color[1], color[2], 1.0f)),
                      4.0f);

  draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                IM_COL32(255, 255, 255, 255), 4.0f, 0, 1.0f);

  if (ImGui::IsItemClicked()) ImGui::OpenPopup(id);

  bool changed = false;
  if (ImGui::BeginPopup(id, ImGuiWindowFlags_NoMove)) {
    changed = ImGui::ColorPicker3(
        "##picker", color,
        ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha |
            ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_PickerHueBar |
            ImGuiColorEditFlags_NoSmallPreview);
    ImGui::EndPopup();
  }
  return changed;
}

bool SettingsPanel::FilePickerButton(const char* label, const ImVec2& size) {
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.22f, 0.27f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.30f, 0.32f, 0.40f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                        ImVec4(0.35f, 0.38f, 0.50f, 1.0f));

  bool pressed = ImGui::Button(label, size);

  ImGui::PopStyleColor(3);

  return pressed;
}

bool SettingsPanel::NumberBox(const char* id, float* value, float width) {
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3.5f));

  ImGui::PushItemWidth(width);
  bool changed = ImGui::InputFloat(id, value, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::PopStyleVar(2);
  return changed;
}
