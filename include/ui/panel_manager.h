#pragma once
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "GLFW/glfw3.h"
#include "panel.h"
#include "ui/panel.h"

namespace ui {

class Views;

class PanelManager {
 public:
  PanelManager() = default;
  void initPanels(GLFWwindow* w);
  void renderPanels(ImGuiWindowClass* window_class);
  void endPanels();
  void addPanel(std::unique_ptr<Panel> panel);

  void setPanelVisible(const std::string& name, bool visible);
  bool isPanelVisible(const std::string& name) const;

  void setGames(const std::vector<Game>* games);
  std::unique_ptr<Views> view_;

 private:
  std::vector<std::unique_ptr<Panel>> panels_;
};

class Views {
 public:
  Views(std::unique_ptr<PanelManager> pm) : panel_manager(std::move(pm)) {}
  ImGuiID ReplaceDockNode();
  void MainMenu();
  void Library();
  void Favorites();
  void Settings();

 private:
  std::unique_ptr<PanelManager> panel_manager;
};

}  // namespace ui
