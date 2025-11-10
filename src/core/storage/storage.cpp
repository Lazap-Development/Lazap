#include "storage/storage.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

#include "utils/fnv1a.h"

namespace fs = std::filesystem;

namespace StorageConfig {
std::string getConfigPath() {
#ifdef _WIN32
  const char* appdata = std::getenv("APPDATA");
  if (appdata) {
    return std::string(appdata);
  }
  return "C:\\ProgramData";
#else
  const char* xdg_config = std::getenv("XDG_CONFIG_HOME");
  if (xdg_config) {
    return std::string(xdg_config);
  }
  const char* home = std::getenv("HOME");
  if (home) {
    return std::string(home) + "/.config";
  }
  return "/tmp";
#endif
}

const std::string CONFIG_DIR = getConfigPath();
const std::string APP_NAME = "lazap";
const std::string CONFIG_NAME = "config.toml";
const std::string STORAGE_DIR = CONFIG_DIR +
#ifdef _WIN32
                                "\\" + APP_NAME;
#else
                                "/" + APP_NAME;
#endif
}  // namespace StorageConfig

Storage::Storage() : storagePath(StorageConfig::STORAGE_DIR) {
  ensureDirectoryExists();
}

void Storage::ensureDirectoryExists() {
  if (!fs::exists(storagePath)) {
    fs::create_directories(storagePath);
  }
}

std::string Storage::getFilePath(const std::string& filename) const {
#ifdef _WIN32
  return storagePath + "\\" + filename;
#else
  return storagePath + "/" + filename;
#endif
}

bool Storage::save(const std::string& filename, const std::string& data) {
  try {
    std::string filepath = getFilePath(filename);
    std::ofstream file(filepath, std::ios::binary);

    if (!file.is_open()) {
      return false;
    }

    file << data;
    file.close();
    return true;
  } catch (const std::exception& e) {
    return false;
  }
}

std::string Storage::load(const std::string& filename) {
  try {
    std::string filepath = getFilePath(filename);

    if (!fs::exists(filepath)) {
      throw std::runtime_error("File does not exist: " + filepath);
    }

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load file: " + std::string(e.what()));
  }
}

bool Storage::exists(const std::string& filename) const {
  return fs::exists(getFilePath(filename));
}

bool Storage::remove(const std::string& filename) {
  try {
    std::string filepath = getFilePath(filename);
    if (fs::exists(filepath)) {
      return fs::remove(filepath);
    }
    return false;
  } catch (const std::exception& e) {
    return false;
  }
}

std::string Storage::getStoragePath() const { return storagePath; }

// Begin TOML++
bool Storage::initTOML() {
  try {
    std::string filepath = getFilePath(StorageConfig::CONFIG_NAME);

    if (fs::exists(filepath)) {
      try {
        toml::table config = loadTOML();

        if (!config.contains("globals") || !config.contains("settings") ||
            !config.contains("games")) {
          throw std::runtime_error("Missing required sections in config");
        }

        return false;
      } catch (const toml::parse_error& e) {
        fs::remove(filepath);
      } catch (const std::exception& e) {
        fs::remove(filepath);
      }
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
      return false;
    }

    toml::table defaultConfig;
    toml::table globals;
    globals.insert("username", "lazap");
    toml::table settings;
    toml::table games;

    defaultConfig.insert("globals", globals);
    defaultConfig.insert("settings", settings);
    defaultConfig.insert("games", games);

    file << defaultConfig;
    file.close();
    return true;
  } catch (const std::exception& e) {
    return false;
  }
}

void Storage::insertGameTOML(const std::string& name) {
  updateTOML([name](toml::table& config) {
    if (!config.contains("games")) {
      config.insert("games", toml::table{});
    }

    auto gamesTable = config["games"].as_table();

    toml::table gameTable;
    gameTable.insert("name", toml::value(name));
    gameTable.insert("favourite", toml::value(false));

    gamesTable->insert(
        std::to_string(fnv1a::hash(name.c_str(), std::strlen(name.c_str()))),
        gameTable);
  });
}

void Storage::updateTOML(const std::function<void(toml::table&)>& modifier) {
  toml::table config = loadTOML();
  modifier(config);
  saveTOML(config);
}

bool Storage::saveTOML(const toml::table& data) {
  try {
    std::string filepath = getFilePath(StorageConfig::CONFIG_NAME);
    std::ofstream file(filepath);
    if (!file.is_open()) {
      return false;
    }
    file << data;
    file.close();
    return true;
  } catch (const std::exception& e) {
    return false;
  }
}

toml::table Storage::loadTOML() {
  try {
    std::string filepath = getFilePath(StorageConfig::CONFIG_NAME);
    if (!fs::exists(filepath)) {
      throw std::runtime_error("File does not exist: " + filepath);
    }
    std::ifstream file(filepath);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + filepath);
    }
    return toml::parse(file);
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load TOML file: " +
                             std::string(e.what()));
  }
}
