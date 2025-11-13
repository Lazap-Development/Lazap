#pragma once

#include <string>
#include <unordered_map>

#include "battery/embed.hpp"

class ImageManager {
 public:
  static unsigned int loadPNG(b::EmbedInternal::EmbeddedFile path,
                              const std::string& id);
  static unsigned int loadSVG(b::EmbedInternal::EmbeddedFile embed,
                              const std::string& id, uint32_t color);
  static unsigned int get(const std::string& name);
  static void clear();

 private:
  static std::unordered_map<std::string, unsigned int> cache;
};

const std::string intToHex(int num);
