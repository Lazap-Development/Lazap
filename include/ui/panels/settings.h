#pragma once
#include <imgui.h>

#include "ui/panel.h"
#include "ui/panels/titlebar.h"

namespace ui {
enum class SettingsView {
  LauncherConfig,
  AccountSettings,
};

enum class InputType {
  Toggle,
  ColorPicker,
  ImagePicker,
  IntTextbox,
  StringTextbox,
};

class SettingsPanel : public Panel {
 public:
  SettingsPanel(Storage* storage, Titlebar* titlebar)
      : Panel("Settings", storage), titlebar_(titlebar) {}

  void init() override;
  void render() override;
  void end() override {};

 private:
  bool quitTrayMin_ = false;
  bool autoStart_ = false;
  bool checkUpdates_ = true;
  bool launcherIcons_ = true;
  bool customTitlebar_ = true;
  bool discordRpc_ = false;

  Titlebar* titlebar_;

  void loadSettings();
  void saveSettings();

  bool addMenuButton(const std::string& label, ImVec2 size, bool active);
  void addIcon(const std::string& icon);
  void addSection(const std::string& title, const std::string& icon);
  bool addOption(const std::string& label, InputType input, bool* value,
                 bool disabled = false);
  bool ToggleButton(const char* label, bool* v, bool disabled);
  bool ColorBox(const char* id, float color[3], ImVec2 size);
  bool FilePickerButton(const char* label, const ImVec2& size);
  bool NumberBox(const char* id, float* value, float width);
  SettingsView view_ = SettingsView::LauncherConfig;
};

}  // namespace ui
