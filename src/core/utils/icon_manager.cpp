// clang-format off
#include "GL/glew.h"
#include "utils/icon_manager.h"
// clang-format on

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
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

  return texture_id;
};

void IconManager::LoadAllIcons(const std::string icon_dir) {
  for (const auto& entry : std::filesystem::directory_iterator(icon_dir)) {
    if (entry.path().extension() == ".png") {
      const std::string& name = entry.path().stem().string();
      if (icon_cache.find(name) != icon_cache.end()) continue;
      GLuint texture_id = LoadIcon(entry.path().string());
      icon_cache[name] = texture_id;
    }
  }
  std::cout << "Loaded " << icon_cache.size() << " icons into cache."
            << std::endl;
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
