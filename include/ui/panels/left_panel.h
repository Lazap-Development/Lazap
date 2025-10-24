#pragma once
#include "ui/panel.h"

namespace ui {

class LeftPanel : public Panel {
 public:
  LeftPanel() : Panel("Left Menu") {};
  void init() override;
  void render() override;
  void end() override {}
  void definePointers() override {};
};

}  // namespace ui