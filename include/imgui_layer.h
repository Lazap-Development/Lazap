#pragma once
#include <clients/client.h>

#include <memory>
#include <vector>

#include "GLFW/glfw3.h"
#include "storage/storage.h"
#include "ui/panel_manager.h"

class ImGuiLayer {
 public:
  ImGuiLayer() : storage_(nullptr), initialized_(false) {}

  void init(GLFWwindow* window, Storage& storage);
  void begin();
  void render();
  void end();
  void shutdown();

  void setGames(const std::vector<Game> games);

 private:
  Storage* storage_;
  std::vector<Game> games_;
  std::unique_ptr<ui::PanelManager> panel_manager_;
  bool initialized_ = false;
};
