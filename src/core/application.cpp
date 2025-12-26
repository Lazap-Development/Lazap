#include "application.h"

#include <cstdio>
#include <memory>
#include <toml++/toml.hpp>
#include <vector>

#include "addons/discord_rpc/discord_rpc.h"
#include "clients/epic_games.h"
#include "clients/riot_games.h"
#include "clients/rockstar_games.h"
#include "clients/steam/steam.h"
#include "clients/ubisoft_connect.h"
#include "imgui_layer.h"
#include "storage/storage.h"
#include "utils/banner_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "clients/custom_games.h"
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

#ifdef _WIN32
void UpdateWindowCorners(GLFWwindow *window) {
  HWND hwnd = glfwGetWin32Window(window);

  WINDOWPLACEMENT placement = {};
  placement.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(hwnd, &placement);

  bool isMaximized = (placement.showCmd == SW_SHOWMAXIMIZED);

  DWM_WINDOW_CORNER_PREFERENCE cornerPref =
      isMaximized ? DWMWCP_DONOTROUND : DWMWCP_ROUND;
  DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPref,
                        sizeof(cornerPref));
}
#endif

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
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  glfwWindowHint(GLFW_SAMPLES, 4);

  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "lazap");
  }

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  float xscale, yscale;
  glfwGetMonitorContentScale(monitor, &xscale, &yscale);

  float sx = 1.0f, sy = 1.0f;

#ifdef _WIN32
  sx = xscale;
  sy = yscale;
#endif

  int windowWidth = WINDOW_SIZE[0] / 1.3 * sx;
  int windowHeight = WINDOW_SIZE[1] / 1.3 * sy;

  GLFWwindow *window =
      glfwCreateWindow(windowWidth, windowHeight, "Lazap", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return;
  }

  glfwSetWindowSizeLimits(window, MIN_WINDOW_SIZE[0], MIN_WINDOW_SIZE[1],
                          GLFW_DONT_CARE, GLFW_DONT_CARE);

  if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
    int windowX = (mode->width - windowWidth) / 2;
    int windowY = (mode->height - windowHeight) / 2;

    glfwSetWindowPos(window, windowX, windowY);
  }

  glfwMakeContextCurrent(window);
  glfwSetMouseButtonCallback(window, WindowCallbacks::mouseButtonCB);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  glEnable(GL_MULTISAMPLE);

  glfwSwapInterval(1);

  Storage storage;
  storage.initTOML();

  bool discordRpc = true;
  auto toml = storage.loadTOML();
  auto *settingsTable = toml["settings"].as_table();
  if (settingsTable) {
    discordRpc = settingsTable->get("discord_rpc")->value_or(false);
  }

  std::vector<std::unique_ptr<Client>> clients;
  clients.push_back(std::make_unique<Steam>());
  clients.push_back(std::make_unique<EpicGames>());
  clients.push_back(std::make_unique<UbisoftConnect>());
  clients.push_back(std::make_unique<RockstarGames>());
  clients.push_back(std::make_unique<RiotGames>());
  clients.push_back(std::make_unique<CustomGames>(storage));

  BannerManager bm;
  auto loadAllGames = [&clients, &bm, &storage]() -> std::vector<Game> {
    std::vector<Game> games;
    for (const auto &client : clients) {
      for (auto &game : client->getInstalledGames()) {
        if (game.bannerUrl.empty())
          game.bannerUrl = bm.getBanner(client->getType(), game.name,
                                        std::to_string(game.appId));
        storage.insertGameTOML(game.name);
        games.push_back(std::move(game));
      }
    }
    return games;
  };

  std::vector<Game> games = loadAllGames();

  ImGuiLayer imgui(window, storage);
  imgui.setGames(std::move(games));

  imgui.setOnGamesReload([&imgui, &loadAllGames]() {
    std::vector<Game> reloadedGames = loadAllGames();
    imgui.setGames(std::move(reloadedGames));
  });

  if (discordRpc) {
    discord::RichPresence::Initialize("932504287337148417");
    discord::RichPresence::UpdatePresence("Lazap", "In Main Menu");
  }

  printf("Startup took: %ld ms\n",
         std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - startupBegin)
             .count());

  glfwShowWindow(window);

#ifdef _WIN32
  // Windows 11 native rounded corners and frame extension
  HWND hwnd = glfwGetWin32Window(window);
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  DWM_WINDOW_CORNER_PREFERENCE cornerPref = DWMWCP_ROUND;
  DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPref,
                        sizeof(cornerPref));
#endif

  RunnerState runner;
  while (!glfwWindowShouldClose(window)) {
    IdleBySleeping(runner.fpsIdling);

    glfwPollEvents();

#ifdef _WIN32
    UpdateWindowCorners(window);
#endif

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

  if (discordRpc) {
    discord::RichPresence::Shutdown();
  }
  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void WindowCallbacks::mouseButtonCB(GLFWwindow *window, int button, int action,
                                    int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    const int edgeThreshold = 10;

    bool nearLeft = (xpos <= edgeThreshold);
    bool nearRight = (xpos >= width - edgeThreshold);
    bool nearTop = (ypos <= edgeThreshold);
    bool nearBottom = (ypos >= height - edgeThreshold);

    if (nearTop && nearLeft) {
      glfwResizeWindow(window, GLFW_WINDOW_TOPLEFT);
    } else if (nearTop && nearRight) {
      glfwResizeWindow(window, GLFW_WINDOW_TOPRIGHT);
    } else if (nearBottom && nearLeft) {
      glfwResizeWindow(window, GLFW_WINDOW_BOTTOMLEFT);
    } else if (nearBottom && nearRight) {
      glfwResizeWindow(window, GLFW_WINDOW_BOTTOMRIGHT);
    }

    else if (nearLeft) {
      glfwResizeWindow(window, GLFW_WINDOW_LEFT);
    } else if (nearRight) {
      glfwResizeWindow(window, GLFW_WINDOW_RIGHT);
    } else if (nearTop) {
      glfwResizeWindow(window, GLFW_WINDOW_TOP);
    } else if (nearBottom) {
      glfwResizeWindow(window, GLFW_WINDOW_BOTTOM);
    }
  }
}
