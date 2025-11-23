#include "ui/panel_manager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ui/panels/game_panel.h"
#include "ui/panels/gameinfo_panel.h"
#include "ui/panels/left_panel.h"
#include "ui/panels/settings.h"
#include "ui/panels/titlebar.h"

using namespace ui;

template <typename T, typename... Args>
std::unique_ptr<T> makePanelWithCB(PanelManager *pm, Args &&...args) {
  auto panel = std::make_unique<T>(std::forward<Args>(args)...);
  std::string panelName = panel->getName();
  panel->setOnRefresh([pm, panelName]() { pm->refreshPanel(panelName); });
  return panel;
}

void PanelManager::initPanels(GLFWwindow *w, Storage &storage) {
  view_ = std::make_unique<Views>(this);

  addPanel(std::make_unique<Titlebar>(w));
  addPanel(std::make_unique<LeftPanel>(view_.get()));
  addPanel(std::make_unique<GameInfoPanel>());

  addPanel(makePanelWithCB<GamePanel>(this, "Library", &view_->view, &storage));
  addPanel(
      makePanelWithCB<GamePanel>(this, "Favorites", &view_->view, &storage));
  addPanel(makePanelWithCB<GamePanel>(this, "Recently Played", &view_->view,
                                      &storage));

  for (auto &panel : panels_) {
    panel->init();
  }
}

void PanelManager::renderPanels(ImGuiWindowClass *window_class) {
  for (auto &panel : panels_) {
    if (panel->visible()) {
      ImGui::SetNextWindowClass(window_class);
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
    if (p->getName() == name) {
      p->setVisible(visible);
      return;
    }
  }
}

bool PanelManager::isPanelVisible(const std::string &name) const {
  for (const auto &p : panels_) {
    if (p->getName() == name) {
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

void PanelManager::refreshPanel(const std::string &name) {
  for (auto &panel : panels_) {
    if (panel->getName() == name) {
      if (auto *gp = dynamic_cast<GamePanel *>(panel.get())) {
        gp->refresh();
      }
      return;
    }
  }
}

void Views::BuildDockLayout() {
  ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
  ImGui::DockBuilderRemoveNode(dockspace_id);
  ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

  ImGuiID titlebar, left, gamesinfo_id, bottom;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.0445f, &left,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.07f, &titlebar,
                              &dockspace_id);
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.466f, &gamesinfo_id,
                              &bottom);

  ImGui::DockBuilderDockWindow("Titlebar", titlebar);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Games Info", gamesinfo_id);
  ImGui::DockBuilderDockWindow("Recently Played", bottom);
  ImGui::DockBuilderDockWindow("Library", bottom);
  ImGui::DockBuilderDockWindow("Favorites", bottom);
  ImGui::DockBuilderDockWindow("Settings", bottom);

  ImGui::DockBuilderFinish(dockspace_id);
}

void Views::MainMenu() {
  if (view == ViewType::MainMenu) return;

  if (!docked_layout_built_) {
    BuildDockLayout();
    docked_layout_built_ = true;
  }

  panel_manager->setPanelVisible("Games Info", true);
  panel_manager->setPanelVisible("Favorites", false);
  panel_manager->setPanelVisible("Library", false);
  panel_manager->setPanelVisible("Recently Played", true);
  panel_manager->setPanelVisible("Settings", false);
  view = ViewType::MainMenu;
}

void Views::Library() {
  if (view == ViewType::Library) return;

  panel_manager->setPanelVisible("Games Info", false);
  panel_manager->setPanelVisible("Favorites", false);
  panel_manager->setPanelVisible("Library", true);
  panel_manager->setPanelVisible("Recently Played", false);
  view = ViewType::Library;
}

void Views::Favorites() {
  if (view == ViewType::Favorites) return;

  panel_manager->setPanelVisible("Games Info", false);
  panel_manager->setPanelVisible("Favorites", true);
  panel_manager->setPanelVisible("Library", false);
  panel_manager->setPanelVisible("Recently Played", false);
  view = ViewType::Favorites;

  panel_manager->refreshPanel("Favorites");
}
