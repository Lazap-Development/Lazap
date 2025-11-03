#include <imgui.h>

#include "utils/font_manager.h"

std::unordered_map<std::string, ImFont*> FontManager::fonts_;

void FontManager::Init() {
  ImGuiIO& io = ImGui::GetIO();
  fonts_["Default"] =
      io.Fonts->AddFontFromFileTTF("src/assets/fonts/Nunito-Medium.ttf", 16.0f);
}

void FontManager::Shutdown() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
  fonts_.clear();
}

ImFont* FontManager::LoadFont(const std::string& name, const std::string& path,
                              float size) {
  ImGuiIO& io = ImGui::GetIO();
  ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), size);
  if (font) {
    fonts_[name] = font;
  } else {
    printf("Unable to load font %s", name.c_str());
  }
  return font;
}

ImFont* FontManager::GetFont(const std::string& name) {
  auto it = fonts_.find(name);
  if (it != fonts_.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}