#include <imgui.h>

#include "battery/embed.hpp"
#include "utils/font_manager.h"

std::unordered_map<std::string, ImFont*> FontManager::fonts_;

void FontManager::init() {
  ImGuiIO& io = ImGui::GetIO();
  b::EmbedInternal::EmbeddedFile embed =
      b::embed<"assets/fonts/Nunito-Medium.ttf">();

  ImFontConfig config;
  config.FontDataOwnedByAtlas = false;

  fonts_["Default"] = io.Fonts->AddFontFromMemoryTTF(
      (void*)embed.data(), static_cast<int>(embed.size()), 16.0f, &config);
}

void FontManager::shutdown() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
  fonts_.clear();
}

ImFont* FontManager::loadFont(const std::string& name,
                              b::EmbedInternal::EmbeddedFile embed,
                              float size) {
  ImGuiIO& io = ImGui::GetIO();

  ImFontConfig config;
  config.FontDataOwnedByAtlas = false;

  ImFont* font = io.Fonts->AddFontFromMemoryTTF(
      (void*)embed.data(), static_cast<int>(embed.size()), size, &config);

  if (font) {
    fonts_[name] = font;
  } else {
    printf("Unable to load font %s\n", name.c_str());
  }
  return font;
}

ImFont* FontManager::getFont(const std::string& name) {
  auto it = fonts_.find(name);
  if (it != fonts_.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}
