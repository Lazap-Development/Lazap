#include <imgui_internal.h>
#include <imgui_layer.h>
#include <utils/launch_manager.h>

#include <filesystem>

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

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::DockSpaceOverViewport(viewport->ID);

  static bool first_time = true;
  if (first_time && !std::filesystem::exists("lazap_imgui.ini")) {
    createInitialDockLayout();
    first_time = false;
    ImGui::SaveIniSettingsToDisk("lazap_imgui.ini");
  }
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiWindowFlags host_flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
      ImGuiWindowFlags_NoMove;

  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowRounding = 0.0f;
  style.WindowBorderSize = 0.0f;
  style.WindowPadding = ImVec2(2.0f, 2.0f);
  ImGui::Begin("HostWindow", nullptr, host_flags);
  ImGui::End();
}

void ImGuiLayer::render() {
  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  panel_manager_->renderPanels(&window_class);
}

void ImGuiLayer::end(GLFWwindow *window) {
  ImGui::Render();
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

void ImGuiLayer::setGames(std::vector<Game> games) {
  games_ = std::move(games);
}

static void createInitialDockLayout() {
  ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;

  ImGui::DockBuilderRemoveNode(dockspace_id);
  ImGui::DockBuilderAddNode(
      dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoTabBar |
                        ImGuiDockNodeFlags_HiddenTabBar |
                        ImGuiDockNodeFlags_NoWindowMenuButton |
                        ImGuiDockNodeFlags_NoResize);
  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

  ImGuiID left, right;
  ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.10f, &left,
                              &right);
  ImGui::DockBuilderDockWindow("Left Menu", left);
  ImGui::DockBuilderDockWindow("Library", right);

  ImGui::DockBuilderFinish(dockspace_id);
}