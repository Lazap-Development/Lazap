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
#include "stb_image.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00  // Windows 10
#include <windows.h>
#ifndef DWMWINDOWCORNERPREFERENCE_DEFINED
typedef enum DWM_WINDOW_CORNER_PREFERENCE {
  DWMWCP_DEFAULT = 0,
  DWMWCP_DONOTROUND = 1,
  DWMWCP_ROUND = 2,
  DWMWCP_ROUNDSMALL = 3
} DWM_WINDOW_CORNER_PREFERENCE;

#define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  glfwSwapInterval(1);

  ResizeState *resizeState = new ResizeState();
  resizeState->cursorArrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  resizeState->cursorResize_NS = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  resizeState->cursorResize_EW = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  resizeState->cursorResize_NESW =
      glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
  resizeState->cursorResize_NWSE =
      glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);

  glfwSetWindowUserPointer(window, resizeState);

  glfwSetMouseButtonCallback(window, WindowCallbacks::mouseButtonCB);
  glfwSetCursorPosCallback(window, WindowCallbacks::cursorPosCB);

  Storage storage;
  storage.initTOML();

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
      storage.insertGameTOML(game.name);
      games.push_back(std::move(game));
    }
  }

  ImGuiLayer imgui(window, resizeState, storage);
  imgui.setGames(std::move(games));

  discord::RichPresence::Initialize("932504287337148417");
  discord::RichPresence::UpdatePresence("Lazap", "In Main Menu");

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

  glfwDestroyCursor(resizeState->cursorArrow);
  glfwDestroyCursor(resizeState->cursorResize_NS);
  glfwDestroyCursor(resizeState->cursorResize_EW);
  glfwDestroyCursor(resizeState->cursorResize_NESW);
  glfwDestroyCursor(resizeState->cursorResize_NWSE);
  delete resizeState;

  discord::RichPresence::Shutdown();
  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void WindowCallbacks::mouseButtonCB(GLFWwindow *window, int button, int action,
                                    int /*mod*/) {
  if (button != GLFW_MOUSE_BUTTON_LEFT) return;

  ResizeState *state =
      static_cast<ResizeState *>(glfwGetWindowUserPointer(window));
  if (!state) return;

  if (action == GLFW_PRESS && state->edge != NONE) {
    state->isDragging = true;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    int winX, winY;
    glfwGetWindowPos(window, &winX, &winY);

    state->startScreenX = winX + cursorX;
    state->startScreenY = winY + cursorY;

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    state->startWidth = w;
    state->startHeight = h;
    state->startPosX = winX;
    state->startPosY = winY;
  } else if (action == GLFW_RELEASE) {
    state->isDragging = false;
  }
}

void WindowCallbacks::cursorPosCB(GLFWwindow *window, double xpos,
                                  double ypos) {
  ResizeState *state =
      static_cast<ResizeState *>(glfwGetWindowUserPointer(window));
  if (!state) return;

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  int detectedEdge = NONE;
  const int border = RESIZE_BORDER;

  if (xpos < border) detectedEdge |= LEFT;
  if (xpos > width - border) detectedEdge |= RIGHT;
  if (ypos < border) detectedEdge |= TOP;
  if (ypos > height - border) detectedEdge |= BOTTOM;

  if (!state->isDragging) {
    state->edge = detectedEdge;
  }

  if (state->isDragging) {
    int currentWinX, currentWinY;
    glfwGetWindowPos(window, &currentWinX, &currentWinY);

    double currentScreenX = currentWinX + xpos;
    double currentScreenY = currentWinY + ypos;
    double deltaX = currentScreenX - state->startScreenX;
    double deltaY = currentScreenY - state->startScreenY;

    int newWidth = state->startWidth;
    int newHeight = state->startHeight;
    int newPosX = state->startPosX;
    int newPosY = state->startPosY;

    if (state->edge & LEFT) {
      int proposed = state->startWidth - (int)deltaX;
      if (proposed >= MIN_WINDOW_SIZE[0]) {
        newWidth = proposed;
        newPosX += (int)deltaX;
      } else {
        newWidth = MIN_WINDOW_SIZE[0];
        newPosX += state->startWidth - MIN_WINDOW_SIZE[0];
      }
    } else if (state->edge & RIGHT) {
      newWidth = std::max(MIN_WINDOW_SIZE[0], state->startWidth + (int)deltaX);
    }

    if (state->edge & TOP) {
      int proposed = state->startHeight - (int)deltaY;
      if (proposed >= MIN_WINDOW_SIZE[1]) {
        newHeight = proposed;
        newPosY += (int)deltaY;
      } else {
        newHeight = MIN_WINDOW_SIZE[1];
        newPosY += state->startHeight - MIN_WINDOW_SIZE[1];
      }
    } else if (state->edge & BOTTOM) {
      newHeight =
          std::max(MIN_WINDOW_SIZE[1], state->startHeight + (int)deltaY);
    }

    glfwSetWindowSize(window, newWidth, newHeight);
    if (state->edge & (LEFT | TOP)) {
      glfwSetWindowPos(window, newPosX, newPosY);
    }
  }

  GLFWcursor *cursor = state->cursorArrow;

  if ((state->edge & TOP && state->edge & LEFT) ||
      (state->edge & BOTTOM && state->edge & RIGHT))
    cursor = state->cursorResize_NWSE;
  else if ((state->edge & TOP && state->edge & RIGHT) ||
           (state->edge & BOTTOM && state->edge & LEFT))
    cursor = state->cursorResize_NESW;
  else if (state->edge & (LEFT | RIGHT))
    cursor = state->cursorResize_EW;
  else if (state->edge & (TOP | BOTTOM))
    cursor = state->cursorResize_NS;

  glfwSetCursor(window, cursor);
}
