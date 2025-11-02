#include <application.h>
#include <clients/epic_games.h>
#include <clients/steam/steam.h>
#include <clients/ubisoft_connect.h>
#include <imgui_layer.h>

#include <memory>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "addons/discord_rpc/discord_rpc.h"
#include "utils/icon_manager.h"

void Application::run() {
  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(1779, 979, "Lazap", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewInit();

  ImGuiLayer imgui;
  imgui.init(window);

  std::vector<std::unique_ptr<Client>> clients;
  clients.push_back(std::make_unique<Steam>());
  clients.push_back(std::make_unique<EpicGames>());
  clients.push_back(std::make_unique<UbisoftConnect>());

  std::vector<Game> games;
  for (const auto &client : clients) {
    std::vector<Game> clientGames = client->getInstalledGames();
    games.insert(games.end(), std::make_move_iterator(clientGames.begin()),
                 std::make_move_iterator(clientGames.end()));
  }

  imgui.setGames(std::move(games));

  IconManager::LoadAllIcons("src/assets/icons/");
  IconManager::LoadAllIcons("src/assets/img/");

  discord::RichPresence::Initialize("932504287337148417");
  discord::RichPresence::UpdatePresence("Lazap", "In Main Menu");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    imgui.begin();
    imgui.render();
    imgui.end(window);

    glfwSwapBuffers(window);
  }

  discord::RichPresence::Shutdown();
  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}
