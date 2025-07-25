#pragma once
#include "../core/clients/client.h"
#include "GLFW/glfw3.h"
#include <vector>

class ImGuiLayer {
public:
  void init(GLFWwindow *window);
  void begin();
  void render();
  void end();
  void shutdown();

  void setGames(const std::vector<Game> &games);

private:
  std::vector<Game> games_;
};
