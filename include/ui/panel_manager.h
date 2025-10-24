#pragma once
#include <memory>
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
  void addPanel(std::unique_ptr<Panel> panel);
  void definePointers();

 private:
  std::vector<std::unique_ptr<Panel>> panels_;
};
}  // namespace ui