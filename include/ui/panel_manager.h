#pragma once
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

#include "clients/client.h"
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
  void definePointers();

  void setPanelVisible(const std::string& name, bool visible);
  bool isPanelVisible(const std::string& name) const;

 private:
  std::vector<std::unique_ptr<Panel>> panels_;
};
}  // namespace ui