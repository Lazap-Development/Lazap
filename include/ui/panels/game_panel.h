#pragma once
#include <memory>

#include "ui/panel.h"

namespace ui {

class GamePanel : public Panel {
 public:
  GamePanel() : Panel("Library") {};
  void init() override;
  void render() override;
  void end() override {}
  std::vector<Game>* games = nullptr;
  void definePointers() override;

 protected:
  std::vector<Game>* games_ = nullptr;
};

using GamePanelPtr = std::unique_ptr<GamePanel>;

}  // namespace ui