// clang-format off
#include "GL/glew.h"
#include "utils/icon_manager.h"
// clang-format on

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <lunasvg.h>

#include "stb_image.h"

std::unordered_map<std::string, GLuint> IconManager::icon_cache;

GLuint IconManager::LoadIcon(const std::string& path) {
  int width, height, channels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
  if (!data) {
    printf("Failed to load icon: %s", path.c_str());
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  icon_cache[std::filesystem::path(path).stem().string()] = texture_id;
  return texture_id;
};

GLuint IconManager::LoadSVG(const std::string& path,
                            const std::string& name = "", uint32_t color = 0) {
  auto svg = lunasvg::Document::loadFromFile(path);
  if (!svg) {
    printf("Failed to load SVG: %s\n", path.c_str());
    return 0;
  }

  if (name != "") {
    auto elements = svg->querySelectorAll("#path");
    for (auto& element : elements) {
      element.setAttribute("color", "#" + intToHex((color >> 16) & 0xFF) +
                                        intToHex((color >> 8) & 0xFF) +
                                        intToHex(color & 0xFF));
    }
  }

  auto bitmap = svg->renderToBitmap((int)svg->height(), (int)svg->width());
  if (!bitmap.valid()) {
    printf("Failed to render SVG: %s\n", path.c_str());
    return 0;
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width(), bitmap.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, bitmap.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (name != "") {
    icon_cache[name] = texture_id;
  } else {
    icon_cache[std::filesystem::path(path).stem().string()] = texture_id;
  }

  return texture_id;
}

GLuint IconManager::GetIcon(const std::string& name) {
  auto it = icon_cache.find(name);
  if (it != icon_cache.end()) {
    return it->second;
  } else {
    return 0;
  }
}

void IconManager::ClearCache() {
  for (const auto& [path, texture_id] : icon_cache) {
    glDeleteTextures(1, &texture_id);
  }
  icon_cache.clear();
}

const std::string intToHex(int num) {
  if (num == 0) return "0";
  std::string hexChars = "0123456789ABCDEF";
  std::string result = "";
  while (num > 0) {
    result = hexChars[num % 16] + result;
    num /= 16;
  }
  return result;
}