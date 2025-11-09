#ifndef STORAGE_H
#define STORAGE_H

#include <functional>
#include <string>
#include <toml++/toml.hpp>

namespace StorageConfig {
std::string getConfigPath();

extern const std::string CONFIG_DIR;
extern const std::string APP_NAME;
extern const std::string STORAGE_DIR;
extern const std::string CONFIG_NAME;
}  // namespace StorageConfig

class Storage {
 private:
  std::string storagePath;

  void ensureDirectoryExists();
  std::string getFilePath(const std::string& filename) const;

 public:
  Storage();
  bool save(const std::string& filename, const std::string& data);
  std::string load(const std::string& filename);
  bool exists(const std::string& filename) const;
  bool remove(const std::string& filename);
  std::string getStoragePath() const;

  bool initTOML();
  void insertGameTOML(const std::string& name);
  void updateTOML(const std::function<void(toml::table&)>& modifier);
  bool saveTOML(const toml::table& data);
  toml::table loadTOML();
};

#endif
