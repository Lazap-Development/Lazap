#include <cstring>
#ifdef _WIN32
#define TRAY_WINAPI
#elif defined(__APPLE__)
#define TRAY_COCOA
#else
#define TRAY_APPINDICATOR
#endif

#include <GLFW/glfw3.h>

#include <tray.h>
#include <utils/tray_manager.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include "battery/embed.hpp"

namespace TrayManager {
static GLFWwindow* win = nullptr;
static bool quit = false;
static bool hidden = false;

static void restore(struct tray_menu* item) {
  if (win) {
    glfwShowWindow(win);
    glfwRestoreWindow(win);
    glfwFocusWindow(win);
    hidden = false;
  }
}

static void on_quit_clicked(struct tray_menu* item) {
  quit = true;
  glfwSetWindowShouldClose(win, GLFW_TRUE);
  glfwPostEmptyEvent();
  tray_exit();
}

static struct tray tray = {
    .icon = "/assets/icons/lazap/lazap.ico",
    .tooltip = "Lazap",
};

void init(GLFWwindow* window) {
  win = window;
  quit = false;

  std::filesystem::path iconPath =
      std::filesystem::temp_directory_path() / "lazap_tray.ico";
  const auto file = b::embed<"assets/icons/lazap/lazap.ico">();

  std::ofstream out(iconPath, std::ios::binary);
  out.write(reinterpret_cast<const char*>(file.data()), file.size());
  out.close();

#ifdef _WIN32
  tray.icon = _strdup(iconPath.string().c_str());
#else
  tray.icon = strdup(iconPath.string().c_str());
#endif

  static struct tray_menu menu[] = {{.text = "Restore App", .cb = restore},
                                    {.text = "-"},
                                    {.text = "Quit", .cb = on_quit_clicked},
                                    {.text = nullptr}};

  tray.menu = menu;

  tray_init(&tray);
}

void update() {
  tray_loop(0);

  if (hidden) {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

void minimize() {
  if (win) {
    glfwHideWindow(win);
    hidden = true;
  }
}

bool shouldQuit() { return quit; }
bool isWindowHidden() { return hidden; }

void shutdown() { tray_exit(); }
}  // namespace TrayManager