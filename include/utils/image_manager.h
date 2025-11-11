#pragma once

#include <string>
#include <unordered_map>

class ImageManager {
 public:
  static unsigned int LoadIcon(const std::string& path);
  static unsigned int LoadSVG(const std::string& path, const std::string& name,
                              uint32_t color);
  static unsigned int GetIcon(const std::string& name);
  static void ClearCache();
  static std::unordered_map<std::string, unsigned int> icon_cache;
};

const std::string intToHex(int num);
