#pragma once
#include <memory>

#include "clients/client.h"
#include "ui/panel.h"

namespace ui {

class GameBox : public Panel {
 public:
  GameBox(const std::string& game_name, Storage* storage)
      : Panel(game_name, storage) {};
  void init(Game game);
  void render() override;
  void end() override {};

 private:
  Game game_;
};

}  // namespace ui
