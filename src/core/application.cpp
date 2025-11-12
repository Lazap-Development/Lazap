#include <application.h>
#include <clients/epic_games.h>
#include <clients/steam/steam.h>
#include <clients/ubisoft_connect.h>
#include <imgui_layer.h>

#include <memory>
#include <toml++/toml.hpp>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "addons/discord_rpc/discord_rpc.h"
#include "clients/riot_games.h"
#include "clients/rockstar_games.h"
#include "storage/storage.h"

double ClockSeconds() {
  using Clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<double>;
  static const auto start = Clock::now();
  return std::chrono::duration_cast<second>(Clock::now() - start).count();
}

void IdleBySleeping(FpsIdling &ioIdling) {
  ioIdling.isIdling = false;

  if ((ioIdling.fpsIdle > 0.f) && ioIdling.enableIdling) {
    double beforeWait = ClockSeconds();
    double waitTimeout = 1.0 / static_cast<double>(ioIdling.fpsIdle);

    glfwWaitEventsTimeout(waitTimeout);

    double afterWait = ClockSeconds();
    double waitDuration = afterWait - beforeWait;
    double waitIdleExpected = 1.0 / ioIdling.fpsIdle;

    ioIdling.isIdling = (waitDuration > waitIdleExpected * 0.9);
  }
}

void Application::run() {
  auto startupBegin = std::chrono::high_resolution_clock::now();

  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  GLFWwindow *window = glfwCreateWindow(1800, 1000, "Lazap", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glewInit();

  Storage storage;
  storage.initTOML();

  ImGuiLayer imgui;
  imgui.init(window, storage);

  std::vector<std::unique_ptr<Client>> clients;
  clients.push_back(std::make_unique<Steam>());
  clients.push_back(std::make_unique<EpicGames>());
  clients.push_back(std::make_unique<UbisoftConnect>());
  clients.push_back(std::make_unique<RockstarGames>());
  clients.push_back(std::make_unique<RiotGames>());

  std::vector<Game> games;
  for (const auto &client : clients) {
    for (const auto &game : client->getInstalledGames()) {
      games.push_back(game);
      storage.insertGameTOML(game.name);
    }
  }

  imgui.setGames(std::move(games));

  discord::RichPresence::Initialize("932504287337148417");
  discord::RichPresence::UpdatePresence("Lazap", "In Main Menu");

  printf("Startup took: %ld ms\n",
         std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - startupBegin)
             .count());

  RunnerState runner;
  while (!glfwWindowShouldClose(window)) {
    IdleBySleeping(runner.fpsIdling);

    glfwPollEvents();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    imgui.begin();
    imgui.render();
    imgui.end();

    glfwSwapBuffers(window);
  }

  discord::RichPresence::Shutdown();
  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}
