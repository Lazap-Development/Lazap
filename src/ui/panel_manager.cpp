#include "ui/panel_manager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ui/panels/game_panel.h"
#include "ui/panels/gameinfo_panel.h"
#include "ui/panels/left_panel.h"
#include "ui/panels/titlebar.h"

using namespace ui;

void PanelManager::initPanels(GLFWwindow *w) {
  view_ =
      std::make_unique<Views>(std::move(std::unique_ptr<PanelManager>(this)));
  PanelManager::addPanel(std::make_unique<Titlebar>(w));
  PanelManager::addPanel(std::make_unique<LeftPanel>(view_.get()));
  PanelManager::addPanel(std::make_unique<GamePanel>("Library"));
  PanelManager::addPanel(std::make_unique<GamePanel>("Favorites"));
  PanelManager::addPanel(std::make_unique<GamePanel>("Recents"));
  PanelManager::addPanel(std::make_unique<GameInfoPanel>());

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
    }
  }
}

ImGuiID Views::ReplaceDockNode() {
  ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;
  ImGui::DockBuilderRemoveNode(dockspace_id);
  ImGui::DockBuilderAddNode(
      dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoTabBar |
                        ImGuiDockNodeFlags_HiddenTabBar |
                        ImGuiDockNodeFlags_NoWindowMenuButton |
                        ImGuiDockNodeFlags_NoResize |
                        ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
  return dockspace_id;
}

void Views::MainMenu() {
  ImGuiID dockspace_id = ReplaceDockNode();

  ImGuiID titlebar, left, gamesinfo_id, bottom;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.04f, &titlebar,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.10f, &left,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.40f, &gamesinfo_id,
                              &bottom);
  ImGui::DockBuilderDockWindow("Titlebar", titlebar);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Games Info", gamesinfo_id);
  ImGui::DockBuilderDockWindow("Recents", bottom);
  ImGui::DockBuilderFinish(dockspace_id);

  panel_manager->setPanelVisible("Left Menu", true);
  panel_manager->setPanelVisible("Games Info", true);
  panel_manager->setPanelVisible("Favorites", false);
  panel_manager->setPanelVisible("Library", false);
  panel_manager->setPanelVisible("Recents", true);
}

void Views::Library() {
  ImGuiID dockspace_id = ReplaceDockNode();

  ImGuiID titlebar, left, right;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.04f, &titlebar,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.10f, &left,
                              &right);
  ImGui::DockBuilderDockWindow("Titlebar", titlebar);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Library", right);

  ImGui::DockBuilderFinish(dockspace_id);

  panel_manager->setPanelVisible("Left Menu", true);
  panel_manager->setPanelVisible("Games Info", false);
  panel_manager->setPanelVisible("Favorites", false);
  panel_manager->setPanelVisible("Library", true);
  panel_manager->setPanelVisible("Recents", false);
}

void Views::Favorites() {
  ImGuiID dockspace_id = ReplaceDockNode();

  ImGuiID titlebar, left, right;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.04f, &titlebar,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.10f, &left,
                              &right);
  ImGui::DockBuilderDockWindow("Titlebar", titlebar);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Favorites", right);

  ImGui::DockBuilderFinish(dockspace_id);

  panel_manager->setPanelVisible("Left Menu", true);
  panel_manager->setPanelVisible("Games Info", false);
  panel_manager->setPanelVisible("Favorites", true);
  panel_manager->setPanelVisible("Library", false);
  panel_manager->setPanelVisible("Recents", false);
}

void Views::Settings() {
  ImGuiID dockspace_id = ReplaceDockNode();

  ImGuiID titlebar, left, center, right;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.04f, &titlebar,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.10f, &left,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.70f, &right,
                              &center);
  ImGui::DockBuilderDockWindow("Titlebar", titlebar);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Settings", center);
  ImGui::DockBuilderDockWindow("Library", right);

  ImGui::DockBuilderFinish(dockspace_id);

  panel_manager->setPanelVisible("Left Menu", true);
  panel_manager->setPanelVisible("Games Info", false);
  panel_manager->setPanelVisible("Favorites", false);
  panel_manager->setPanelVisible("Library", false);
  panel_manager->setPanelVisible("Recents", false);
}