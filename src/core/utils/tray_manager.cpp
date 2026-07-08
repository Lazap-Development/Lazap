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

static void restore(struct tray_menu*) {
  if (win) {
    glfwShowWindow(win);
    glfwRestoreWindow(win);
    glfwFocusWindow(win);
    hidden = false;
  }
}

static void on_quit_clicked(struct tray_menu*) {
  quit = true;
  glfwSetWindowShouldClose(win, GLFW_TRUE);
  glfwPostEmptyEvent();
  tray_exit();
}

static struct tray tray = {
    .icon = nullptr,
    .tooltip = "Lazap",
    .notification_icon = nullptr,
    .notification_text = nullptr,
    .notification_title = nullptr,
    .notification_cb = nullptr,
    .cb = nullptr,
    .menu = nullptr,
    .iconPathCount = 0,
};

static struct tray_menu menu[] = {
    {.text = "Restore App",
     .disabled = 0,
     .checked = 0,
     .checkbox = 0,
     .cb = restore,
     .context = nullptr,
     .submenu = nullptr},
    {.text = "-",
     .disabled = 0,
     .checked = 0,
     .checkbox = 0,
     .cb = nullptr,
     .context = nullptr,
     .submenu = nullptr},
    {.text = "Quit",
     .disabled = 0,
     .checked = 0,
     .checkbox = 0,
     .cb = on_quit_clicked,
     .context = nullptr,
     .submenu = nullptr},
};

void init(GLFWwindow* window) {
  win = window;
  quit = false;

#ifdef _WIN32
  std::filesystem::path iconPath =
      std::filesystem::temp_directory_path() / "lazap_tray.ico";
  const auto file = b::embed<"assets/icons/lazap/lazap.ico">();
#else
  std::filesystem::path iconPath =
      std::filesystem::temp_directory_path() / "lazap_tray.png";
  const auto file = b::embed<"assets/icons/lazap/lazap.png">();
#endif

  std::ofstream out(iconPath, std::ios::binary);
  out.write(reinterpret_cast<const char*>(file.data()), file.size());
  out.close();

#ifdef _WIN32
  tray.icon = _strdup(iconPath.string().c_str());
#else
  tray.icon = strdup(iconPath.string().c_str());
#endif

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
