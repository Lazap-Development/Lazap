#pragma once
#include "ui/panel.h"
#include "ui/panel_manager.h"

namespace ui {

class LeftPanel : public Panel {
 public:
  LeftPanel(Views* v) : Panel("Left Menu"), view_(v) {}

  void init() override;
  void render() override;
  void end() override {}

 private:
  Views* view_ = nullptr;
};

}  // namespace ui
