#include <imgui_layer.h>
#include <utils/launch_manager.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "ui/panel_manager.h"
#include "ui/panels/game_panel.h"
#include "ui/panels/left_panel.h"
#include "ui/themes/themes.h"

void ImGuiLayer::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  Themes::setDefaultDarkColors();

  panel_manager_ = std::make_unique<ui::PanelManager>();
  panel_manager_->initPanels();
  panel_manager_->definePointers();
}

void ImGuiLayer::setGames(std::vector<Game> games) {
  games_ = std::move(games);
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiID dockspace_id = ImGui::GetID("MainWindow");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
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

  panel_manager_->renderPanels();
}

void ImGuiLayer::end(GLFWwindow *window) {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  GLFWwindow *backup_current_context = glfwGetCurrentContext();
  ImGui::UpdatePlatformWindows();
  ImGui::RenderPlatformWindowsDefault();
  glfwMakeContextCurrent(backup_current_context);
}

void ImGuiLayer::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
