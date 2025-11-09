#pragma once

#include <string>
#include <unordered_map>

class IconManager {
 public:
  static unsigned int LoadIcon(const std::string& path);
  static unsigned int LoadSVG(const std::string& path);
  static void LoadAllIcons();
  static GLuint GetIcon(const std::string& name);
  static void ClearCache();
  static std::unordered_map<std::string, GLuint> icon_cache;
};
