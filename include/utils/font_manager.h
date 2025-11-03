#pragma once

#include <string>
#include <unordered_map>

#include "imgui.h"

class FontManager {
 public:
  static void Init();
  static void Shutdown();
  static ImFont* LoadFont(const std::string& name, const std::string& path,
                          float size);
  static ImFont* GetFont(const std::string& name);

 private:
  static std::unordered_map<std::string, ImFont*> fonts_;
};