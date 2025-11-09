#pragma once

#include <string>
#include <unordered_map>

class IconManager {
 public:
  static GLuint LoadIcon(const std::string& path);
  static GLuint LoadSVG(const std::string& path, const std::string& name,
                        uint32_t color);
  static void LoadAllIcons();
  static GLuint GetIcon(const std::string& name);
  static void ClearCache();
  static std::unordered_map<std::string, GLuint> icon_cache;
};

const std::string intToHex(int num);