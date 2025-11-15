
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

 private:
  Game game_;
  GLuint bannerTexture_ = 0;
};

}  // namespace ui
