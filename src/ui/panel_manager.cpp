#include "ui/panel_manager.h"

#include <imgui.h>

#include "ui/panels/game_panel.h"
#include "ui/panels/left_panel.h"

using namespace ui;

void PanelManager::initPanels() {
  PanelManager::addPanel(std::make_unique<LeftPanel>());
  PanelManager::addPanel(std::make_unique<GamePanel>());

  for (auto &panel : panels_) {
    panel->init();
  }
}

void PanelManager::renderPanels(ImGuiWindowClass *window_class) {
  for (auto &panel : panels_) {
    if (panel->visible()) {
      ImGui::SetNextWindowClass(window_class);  // removes tab bar
      panel->render();
    }
  }
}

void PanelManager::endPanels() { panels_.clear(); }

void PanelManager::addPanel(std::unique_ptr<Panel> panel) {
  panels_.push_back(std::move(panel));
}

void PanelManager::setPanelVisible(const std::string &name, bool visible) {
  for (auto &p : panels_) {
    if (p->name() == name) {
      p->setVisible(visible);
      return;
    }
  }
}

bool PanelManager::isPanelVisible(const std::string &name) const {
  for (const auto &p : panels_) {
    if (p->name() == name) {
      return p->visible();
    }
  }
  return false;
}

void PanelManager::setGames(const std::vector<Game> *games) {
  for (auto &panel : panels_) {
    if (auto *gamePanel = dynamic_cast<ui::GamePanel *>(panel.get())) {
      gamePanel->setGames(games);
      return;
    }
  }
}
