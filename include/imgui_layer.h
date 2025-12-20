#pragma once
#include <clients/client.h>

#include <memory>
#include <vector>

#include "storage/storage.h"
#include "ui/panel_manager.h"

class ImGuiLayer {
 public:
  ImGuiLayer(GLFWwindow* window, Storage& storage);

  void begin();
  void render();
  void end();
  void shutdown();

  void setGames(const std::vector<Game> games);

 private:
  Storage* storage_;
  std::vector<Game> games_;
  std::unique_ptr<ui::PanelManager> panel_manager_;
  void renderBackground(const ImGuiViewport* viewport);
  bool initialized_ = false;
};
