#pragma once

#include <string>
#include <unordered_map>

#include "battery/embed.hpp"
#include "imgui.h"

class FontManager {
 public:
  static void init();
  static void shutdown();
  static ImFont* loadFont(const std::string& name,
                          b::EmbedInternal::EmbeddedFile embed, float size);
  static ImFont* getFont(const std::string& name);

 private:
  static std::unordered_map<std::string, ImFont*> fonts_;
};
