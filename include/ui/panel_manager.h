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

enum class ViewType { None, MainMenu, Library, Favorites, Settings };

class Views {
 public:
  explicit Views(PanelManager* pm) : panel_manager(pm) {}
  void BuildDockLayout();
  void MainMenu();
  void Library();
  void Favorites();
  void Settings();
  ViewType view = ViewType::None;

 private:
  PanelManager* panel_manager;
  bool docked_layout_built_ = false;
};

}  // namespace ui
