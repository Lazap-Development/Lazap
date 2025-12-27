#pragma once

#include "clients/client.h"
#include "ui/panel.h"
#include "utils/image_manager.h"

namespace ui {

class GameBox : public Panel {
 public:
  GameBox(const Game& game, Storage* storage);

  void render() override;
  void end() override {};

  void requestRefresh() { requestRefresh_ = true; }

  const Game& getGame() const { return game_; }

  bool requestRefresh_ = false;

 private:
  Game game_;
  Texture bannerTexture_;
};

}  // namespace ui
