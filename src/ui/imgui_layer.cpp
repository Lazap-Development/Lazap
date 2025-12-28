#include <imgui_internal.h>
#include <imgui_layer.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "ui/panel_manager.h"
#include "ui/theme.h"
#include "utils/font_manager.h"
#include "utils/image_manager.h"

ImGuiLayer::ImGuiLayer(GLFWwindow *window, Storage &storage)
    : storage_(&storage), initialized_(false) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigDpiScaleFonts = true;
  io.ConfigDpiScaleViewports = true;
  io.IniFilename = nullptr;

  FontManager::init();
  FontManager::loadFont("Titlebar:Title",
                        b::embed<"assets/fonts/Nunito-SemiBold.ttf">(), 16.0f);

  FontManager::loadFont("Username",
                        b::embed<"assets/fonts/Oxanium-Regular.ttf">(), 15.0f);

  FontManager::loadFont("GameInfo:Title",
                        b::embed<"assets/fonts/ZenDots-Regular.ttf">(), 64.0f);
  FontManager::loadFont("GameInfo:Paragraph",
                        b::embed<"assets/fonts/Nunito-Medium.ttf">(), 18.0f);

  FontManager::loadFont("Left:Button",
                        b::embed<"assets/fonts/Oxanium-Regular.ttf">(), 18.0f);
  FontManager::loadFont(
      "Title", b::embed<"assets/fonts/ArchivoBlack-Regular.ttf">(), 24.0f);
  FontManager::loadFont("GameBox:Title",
                        b::embed<"assets/fonts/Nunito-SemiBold.ttf">(), 17.0f);
  FontManager::loadFont(
      "GameBox:Time", b::embed<"assets/fonts/RobotoMono-Medium.ttf">(), 10.0f);

  FontManager::loadFont("Settings:Button",
                        b::embed<"assets/fonts/Nunito-Bold.ttf">(), 13.0f);
  FontManager::loadFont("Settings:Setting",
                        b::embed<"assets/fonts/Nunito-Light.ttf">(), 18.0f);
  FontManager::loadFont("Settings:Option",
                        b::embed<"assets/fonts/Nunito-SemiBold.ttf">(), 13.0f);

  FontManager::loadFont("Dialog:Title",
                        b::embed<"assets/fonts/ArchivoBlack-Regular.ttf">(),
                        27.0f);
  FontManager::loadFont("Dialog:Button",
                        b::embed<"assets/fonts/Nunito-SemiBold.ttf">(), 15.0f);
  FontManager::loadFont("Dialog:Paragraph",
                        b::embed<"assets/fonts/Oxanium-Regular.ttf">(), 15.0f);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  panel_manager_ = std::make_unique<ui::PanelManager>();
  panel_manager_->initPanels(window, storage);
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiWindowFlags host_flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
      ImGuiWindowFlags_NoMove;

  Themes::setDefaultDarkColors();
  ImGui::Begin("HostWindow", nullptr, host_flags);
  if (initialized_ == false) {
    initialized_ = true;
    panel_manager_->view_->MainMenu();
  }

  renderBackground(viewport);

  ImGui::DockSpace(ImGui::GetID("MainDockSpace"), ImVec2(0.0f, 0.0f),
                   ImGuiDockNodeFlags_None, nullptr);
  ImGui::End();
}

void ImGuiLayer::render() {
  // ImGui::ShowStyleEditor(&ImGui::GetStyle());
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet =
      ImGuiDockNodeFlags_NoTabBar |
      (ImGuiDockNodeFlags)ImGuiDockNodeFlags_NoResize;

  panel_manager_->renderPanels(&window_class);
}

void ImGuiLayer::end() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  GLFWwindow *backup_current_context = glfwGetCurrentContext();
  ImGui::UpdatePlatformWindows();
  ImGui::RenderPlatformWindowsDefault();
  glfwMakeContextCurrent(backup_current_context);
}

void ImGuiLayer::shutdown() {
  panel_manager_->endPanels();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::setGames(std::vector<Game> games) {
  games_ = std::move(games);
  panel_manager_->setGames(&games_);
}

void ImGuiLayer::setOnGamesReload(std::function<void()> callback) {
  onGamesReload_ = callback;
  if (panel_manager_) {
    panel_manager_->setOnGamesReload(callback);
  }
}

void ImGuiLayer::renderBackground(const ImGuiViewport *viewport) {
  if (panel_manager_->view_->view == ui::ViewType::MainMenu) {
    ImDrawList *bg = ImGui::GetWindowDrawList();
    float gameInfoY = viewport->Size.y * 0.466f;
    ImVec2 img_pos = ImVec2(0.0f, 0.0f);
    ImVec2 img_size = ImVec2(viewport->Size.x, gameInfoY);
    // printf("Background size: %.1f x %.1f\n", img_size.x, img_size.y);
    bg->AddImage((ImTextureID)(intptr_t)ImageManager::get("banner"), img_pos,
                 img_size, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE);
    bg->AddRectFilledMultiColor(img_pos, img_size, IM_COL32(0, 0, 0, 0),
                                IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 255),
                                IM_COL32(0, 0, 0, 255));
  }
}
