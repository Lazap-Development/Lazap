#include <application.h>
#include <imgui_layer.h>

#include <cstdio>
#include <memory>
#include <toml++/toml.hpp>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "addons/discord_rpc/discord_rpc.h"
#include "clients/epic_games.h"
#include "clients/riot_games.h"
#include "clients/rockstar_games.h"
#include "clients/steam/steam.h"
#include "clients/ubisoft_connect.h"
#include "storage/storage.h"
#include "utils/banner_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00  // Windows 10
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

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

  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "lazap");
  }

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  int windowWidth = 1440;
  int windowHeight = 800;
  int windowX = (mode->width - windowWidth) / 2;
  int windowY = (mode->height - windowHeight) / 2;

  GLFWwindow *window =
      glfwCreateWindow(windowWidth, windowHeight, "Lazap", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return;
  }

  glfwSetWindowPos(window, windowX, windowY);
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

  BannerManager bm;
  std::vector<Game> games;
  for (const auto &client : clients) {
    for (auto &game : client->getInstalledGames()) {
      game.bannerUrl = bm.getBanner(client->getType(), game.name,
                                    std::to_string(game.appId));
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

#ifdef _WIN32
  // Windows 11 native rounded corners and frame extension
  HWND hwnd = glfwGetWin32Window(window);
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  MARGINS margins = {1, 1, 1, 1};
  DwmExtendFrameIntoClientArea(hwnd, &margins);

  DWM_WINDOW_CORNER_PREFERENCE cornerPref = DWMWCP_ROUND;
  DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPref,
                        sizeof(cornerPref));
#endif

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
