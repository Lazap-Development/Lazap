#pragma once
#include <clients/client.h>

#include <memory>
#include <vector>

#include "GLFW/glfw3.h"
#include "ui/panel_manager.h"

class ImGuiLayer {
 public:
  void init(GLFWwindow* window);
  void begin();
  void render();
  void end(GLFWwindow* window);
  void shutdown();

  void setGames(const std::vector<Game> games);

 private:
  std::vector<Game> games_;
  std::unique_ptr<ui::PanelManager> panel_manager_;
};
