#pragma once
#include <memory>

#include "ui/panel.h"

namespace ui {

class GamePanel : public Panel {
 public:
  GamePanel(const std::string& name) : Panel(name) {};
  void init() override;
  void render() override;
  void end() override {}
  void setGames(const std::vector<Game>* games);

 protected:
  const std::vector<Game>* games_ = nullptr;
};

using GamePanelPtr = std::unique_ptr<GamePanel>;

}  // namespace ui
