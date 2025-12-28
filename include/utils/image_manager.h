#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>

#include "battery/embed.hpp"

struct Texture {
  GLuint id;
  int width;
  int height;
};

class ImageManager {
 public:
  static Texture loadPNG(b::EmbedInternal::EmbeddedFile path,
                         const std::string& id);
  static Texture loadPNG(const std::string& path);

  static GLuint loadSVG(b::EmbedInternal::EmbeddedFile embed,
                        const std::string& id, uint32_t color);
  static GLuint get(const std::string& name);
  static void clear();

 private:
  static std::unordered_map<std::string, unsigned int> cache;
};

const std::string intToHex(int num);
