#pragma once
#include <clients/client.h>

#include <memory>
#include <vector>

#include "GLFW/glfw3.h"
#include "ui/panel.h"

class ImGuiLayer {
 public:
  void init(GLFWwindow *window);
  void begin();
  void render();
  void end();
  void shutdown();

  void setGames(const std::vector<Game> games);
  void addPanel(std::unique_ptr<ui::Panel> panel);

 private:
  std::vector<Game> games_;
  std::vector<std::unique_ptr<ui::Panel>> panels_;
};
