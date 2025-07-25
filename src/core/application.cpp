#include "application.h"
#include "../ui/imgui_layer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "clients/steam.h"
#include <vector>

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

  Steam steam;
  std::vector<Game> games = steam.getInstalledGames();
  imgui.setGames(games);

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

  imgui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}
