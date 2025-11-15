#pragma once
#include <memory>

#include "clients/client.h"
#include "ui/panel.h"

namespace ui {

class GameBox : public Panel {
 public:
  GameBox(const Game& game, Storage* storage)
      : Panel(game.name, storage), game_(game) {};
  void render() override;
  void end() override {};

 private:
  Game game_;
};

}  // namespace ui
