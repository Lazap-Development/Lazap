#pragma once

#include <GL/gl.h>

#include "clients/client.h"
#include "ui/panel.h"
#include "utils/image_manager.h"

namespace ui {

class GameBox : public Panel {
 public:
  GameBox(const Game& game, Storage* storage)
      : Panel(game.name, storage), game_(game) {
    bannerTexture_ = ImageManager::loadPNG(game_.bannerUrl);
  }

  void render() override;
  void end() override {};

  void requestRefresh() { requestRefresh_ = true; }

  bool requestRefresh_ = false;

 private:
  Game game_;
  Texture bannerTexture_;
};

}  // namespace ui
