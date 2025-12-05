#include "utils/image_manager.h"

#include <lunasvg.h>

#include <cstdio>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "battery/embed.hpp"
#include "stb_image.h"

std::unordered_map<std::string, GLuint> ImageManager::cache;

GLuint ImageManager::loadPNG(b::EmbedInternal::EmbeddedFile embed,
                             const std::string& id) {
  const auto data = embed.data();
  const size_t size = embed.size();

  int width, height, channels;
  unsigned char* image_data = stbi_load_from_memory(
      reinterpret_cast<const stbi_uc*>(data), static_cast<int>(size), &width,
      &height, &channels, 4);

  if (!image_data) {
    printf("Failed to load icon: %s\n", stbi_failure_reason());
    return 0;
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(image_data);

  cache[id] = texture_id;

  return texture_id;
}

Texture ImageManager::loadPNG(b::EmbedInternal::EmbeddedFile embed) {
  const auto data = embed.data();
  const size_t size = embed.size();

  int width, height, channels;
  unsigned char* image_data = stbi_load_from_memory(
      reinterpret_cast<const stbi_uc*>(data), static_cast<int>(size), &width,
      &height, &channels, 4);

  if (!image_data) {
    printf("Failed to load embedded PNG: %s\n", stbi_failure_reason());
    return {0, 0, 0};
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(image_data);

  return {texture_id, width, height};
}

Texture ImageManager::loadPNG(const std::string& path) {
  int width, height, channels;

  unsigned char* image_data =
      stbi_load(path.c_str(), &width, &height, &channels, 4);

  if (!image_data) {
    printf("Failed to load PNG '%s': %s\n", path.c_str(),
           stbi_failure_reason());
    return {0, 0, 0};
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(image_data);

  cache[std::filesystem::path(path).stem().string()] = texture_id;

  return {texture_id, width, height};
}

GLuint ImageManager::loadSVG(b::EmbedInternal::EmbeddedFile embed,
                             const std::string& id, uint32_t color = 0) {
  auto svg = lunasvg::Document::loadFromData(embed);
  if (!svg) {
    printf("Failed to load SVG: %s\n", id.c_str());
    return 0;
  }

  auto elements = svg->querySelectorAll("path");
  for (auto& element : elements) {
    std::string hexColor = "#" + intToHex((color >> 16) & 0xFF) +
                           intToHex((color >> 8) & 0xFF) +
                           intToHex(color & 0xFF);

    auto fillAttr = element.getAttribute("fill");
    if (!fillAttr.empty() && fillAttr != "none") {
      element.setAttribute("fill", hexColor);
    }

    auto strokeAttr = element.getAttribute("stroke");
    if (!strokeAttr.empty() && strokeAttr != "none") {
      element.setAttribute("stroke", hexColor);
    }
  }

  auto bitmap = svg->renderToBitmap((int)svg->height(), (int)svg->width());
  if (!bitmap.valid()) {
    printf("Failed to render SVG: %s\n", id.c_str());
    return 0;
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width(), bitmap.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, bitmap.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  cache[id] = texture_id;

  return texture_id;
}

GLuint ImageManager::get(const std::string& name) {
  auto it = cache.find(name);
  if (it != cache.end()) {
    return it->second;
  } else {
    return 0;
  }
}

void ImageManager::clear() {
  for (const auto& [path, texture_id] : cache) {
    glDeleteTextures(1, &texture_id);
  }
  cache.clear();
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
