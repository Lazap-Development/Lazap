#pragma once
#include <memory>

#include "storage/storage.h"
#include "ui/cmps/addgame_dialog.h"
#include "ui/cmps/game_box.h"
#include "ui/panel.h"
#include "ui/panel_manager.h"

namespace ui {

class GamePanel : public Panel {
 public:
  GamePanel(const std::string& name, ViewType* v, Storage* storage = nullptr)
      : Panel(name, storage), view_(v) {}

  void init() override;
  void render() override;
  void end() override {}
  void setGames(const std::vector<Game>* games);
  void refresh() { setGames(games_); }

 private:
  std::vector<std::unique_ptr<GameBox>> gameBoxes_;
  std::unique_ptr<AddGameDialog> addGameDialog_;
  const std::vector<Game>* games_ = nullptr;
  ViewType* view_ = nullptr;
  char searchBuffer_[128];
};

using GamePanelPtr = std::unique_ptr<GamePanel>;

}  // namespace ui
