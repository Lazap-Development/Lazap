#pragma once
#include <imgui.h>

#include <memory>

#include "ui/panel.h"

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
  SettingsPanel() : Panel("Settings") {};

  void init() override;
  void render() override;
  void end() override {};

 private:
  bool addMenuButton(const std::string& label, ImVec2 size, bool active);
  void addIcon(const std::string& icon);
  void addSection(const std::string& title, const std::string& icon);
  void addOption(const std::string& label, InputType input);
  bool ToggleButton(const char* label, bool* v, const ImVec2& size);
  bool ColorBox(const char* id, float color[3], ImVec2 size);
  bool FilePickerButton(const char* label, const ImVec2& size);
  bool NumberBox(const char* id, float* value, float width);
  SettingsView view_ = SettingsView::LauncherConfig;
};

}  // namespace ui
