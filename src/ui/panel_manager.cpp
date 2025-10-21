#include "ui/panel_manager.h"

#include "clients/client.h"
#include "ui/panels/game_panel.h"
#include "ui/panels/left_panel.h"

using namespace ui;

void PanelManager::initPanels() {
  panels_.push_back(new LeftPanel());
  panels_.push_back(new GamePanel());

  for (auto &panel : panels_) {
    panel->init();
  }
}

void PanelManager::renderPanels() {
  for (auto &panel : panels_) {
    panel->render();
  }
}

void PanelManager::endPanels() { panels_.clear(); }

void PanelManager::definePointers() {
  for (auto &panel : panels_) {
    panel->definePointers();
  }
}

void PanelManager::addPanel(Panel *panel) { panels_.push_back(panel); }

void PanelManager::setGames(std::vector<Game> games) { games_ = games; }
