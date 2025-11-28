#pragma once
#include <clients/client.h>

#include <memory>
#include <vector>

#include "application.h"
#include "storage/storage.h"
#include "ui/panel_manager.h"

class ImGuiLayer {
 public:
  ImGuiLayer(GLFWwindow* window, ResizeState* resize_state, Storage& storage);

  void begin();
  void render();
  void end();
  void shutdown();

  void setGames(const std::vector<Game> games);

 private:
  Storage* storage_;
  ResizeState* resize_state_;
  std::vector<Game> games_;
  std::unique_ptr<ui::PanelManager> panel_manager_;
  bool initialized_ = false;
};
