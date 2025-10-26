#include "ui/panels/left_panel.h"

#include "imgui.h"

using namespace ui;

void LeftPanel::init() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8.0f;
  style.FrameRounding = 5.0f;
  style.FramePadding = ImVec2(8.0f, 6.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
}

void LeftPanel::render() {
  if (!visible()) return;

  ImGui::Begin(name_.c_str(), nullptr,
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoTitleBar);
  bool home = ImGui::Button("Home");
  bool favs = ImGui::Button("Favorites");
  bool library = ImGui::Button("All Games");

  if (home == true && view_->view != ViewType::MainMenu) {
    view_->MainMenu();
  }
  if (favs == true && view_->view != ViewType::Favorites) {
    view_->Favorites();
  }
  if (library == true && view_->view != ViewType::Library) {
    view_->Library();
  }
  ImGui::Separator();
  ImGui::End();
}
