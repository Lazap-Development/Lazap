#include <application.h>
#include <clients/steam/steam.h>
#include <imgui_layer.h>

#include <memory>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "addons/discord_rpc/RichPresence.h"

void Application::run() {
  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Lazap", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewInit();

  ImGuiLayer imgui;
  imgui.init(window);

  std::vector<std::unique_ptr<Client>> clients;
  clients.push_back(std::make_unique<Steam>());

  std::vector<Game> games;
  for (const auto &client : clients) {
    std::vector<Game> clientGames = client->getInstalledGames();
    games.insert(games.end(), std::make_move_iterator(clientGames.begin()),
                 std::make_move_iterator(clientGames.end()));
  }

  imgui.setGames(std::move(games));

  // TODO: Add Addon(s) Instantiations here

  RichPresence::Initialize("932504287337148417");
  RichPresence::UpdatePresence("In Main Menu", "Lazap");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    imgui.begin();
    imgui.render();
    imgui.end();

    glfwSwapBuffers(window);
  }

  RichPresence::Shutdown();
  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}
