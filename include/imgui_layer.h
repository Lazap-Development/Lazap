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
  void end();
  void shutdown();

  void setGames(const std::vector<Game> games);

 private:
  std::vector<Game> games_;
  ui::PanelManager panel_manager_;
};
