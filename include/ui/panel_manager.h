#pragma once
#include <vector>

#include "clients/client.h"
#include "panel.h"
#include "ui/panel.h"

namespace ui {
class PanelManager {
 public:
  PanelManager() = default;
  void initPanels();
  void renderPanels();
  void endPanels();
  void addPanel(Panel *panel);
  void setGames(std::vector<Game> games);
  void definePointers();

 private:
  std::vector<Panel *> panels_;
  std::vector<Game> games_;
};
}  // namespace ui