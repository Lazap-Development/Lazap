#pragma once
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "panel.h"
#include "ui/panel.h"

namespace ui {
class PanelManager {
 public:
  PanelManager() = default;
  void initPanels();
  void renderPanels(ImGuiWindowClass* window_class);
  void endPanels();
  void addPanel(std::unique_ptr<Panel> panel);

  void setPanelVisible(const std::string& name, bool visible);
  bool isPanelVisible(const std::string& name) const;

  void setGames(const std::vector<Game>* games);

 private:
  std::vector<std::unique_ptr<Panel>> panels_;
};
}  // namespace ui
