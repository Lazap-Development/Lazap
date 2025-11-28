#pragma once
#include <imgui.h>

#include <memory>

#include "ui/panel.h"

namespace ui {
enum class SettingsView {
  LauncherConfig,
  AccountSettings,
};

class SettingsPanel : public Panel {
 public:
  SettingsPanel() : Panel("Settings") {};
  void init() override;
  void render() override;
  void end() override {};

 private:
  SettingsView view_ = SettingsView::LauncherConfig;
};

}  // namespace ui

void addIcon(const std::string& icon, ImVec2 scale);