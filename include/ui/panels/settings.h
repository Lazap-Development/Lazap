#pragma once
#include <memory>

#include "ui/panel.h"

namespace ui {

class SettingsPanel : public Panel {
 public:
  SettingsPanel() : Panel("Settings") {};
  void init() override {};
  void render() override;
  void end() override {};
};

}  // namespace ui
