#include <imgui_layer.h>
#include <utils/launch_manager.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "ui/panel.h"
#include "ui/themes/themes.h"

void ImGuiLayer::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  Themes::setDefaultDarkColors();
}

void ImGuiLayer::setGames(std::vector<Game> games) {
  games_ = std::move(games);
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::render() {
  // ImGui::Begin("Window A", nullptr, ImGuiWindowFlags_NoCollapse);

  // for (auto &game : games_) {
  //   LaunchManager lm = LaunchManager(game);
  //   if (ImGui::Button(game.name.c_str())) lm.launch();

  //   if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
  //     if (lm.isRunning()) {
  //       lm.kill();
  //     }
  //   }
  // }

  // ImGui::End();

  for (auto &panel : panels_) {
    panel->render();
  }
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

void ImGuiLayer::addPanel(std::unique_ptr<ui::Panel> panel) {
  if (!panel) return;
  panel->definePointers();
  panel->init();
  panels_.push_back(std::move(panel));
}
