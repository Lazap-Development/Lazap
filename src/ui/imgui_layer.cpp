#include "imgui_layer.h"
#include "../core/utils/launch_manager.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

void ImGuiLayer::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::setGames(const std::vector<Game> &games) { games_ = games; }

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::render() {
  ImGui::Begin("Window A", nullptr, ImGuiWindowFlags_NoCollapse);

  for (const auto &game : games_) {
    if (ImGui::Button(game.name.c_str())) {
      LaunchManager::launchGame(game.launcher, game.launchArgs);
    }
  }

  ImGui::End();
}

void ImGuiLayer::end() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
