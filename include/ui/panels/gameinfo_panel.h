#pragma once
#include <memory>

#include "ui/panel.h"

namespace ui {

class GameInfoPanel : public Panel {
 public:
  GameInfoPanel() : Panel("Games Info") {};
  void init() {};
  void render() override;
  void end() override {};
};

}  // namespace ui
