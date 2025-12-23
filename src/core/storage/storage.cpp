// TODO: Use structs for game and mainconf

#include "storage/storage.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

#include "storage/storage_config.h"
#include "ui/themes/themes.h"
#include "utils/fnv1a.h"

namespace fs = std::filesystem;

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

        auto settingsTable = config["settings"].as_table();
        if (settingsTable) {
          if (!settingsTable->contains("quit_tray_min") ||
              !settingsTable->contains("auto_start") ||
              !settingsTable->contains("check_updates") ||
              !settingsTable->contains("launcher_icons") ||
              !settingsTable->contains("discord_rpc") ||
              !settingsTable->contains("accent_color") ||
              !settingsTable->contains("custom_titlebar")) {
            throw std::runtime_error(
                "Settings table is missing required fields");
          }
        }

        auto gamesTable = config["games"].as_table();
        if (gamesTable) {
          for (const auto& [gameId, gameValue] : *gamesTable) {
            if (!gameValue.is_table()) {
              throw std::runtime_error("Invalid game entry: not a table");
            }

            auto gameTable = gameValue.as_table();
            if (!gameTable->contains("name") ||
                !gameTable->contains("favourite") ||
                !gameTable->contains("playtime") ||
                !gameTable->contains("last_launch")) {
              throw std::runtime_error("Game table is missing required fields");
            }

            if (!gameTable->at("name").is_string() ||
                !gameTable->at("favourite").is_boolean() ||
                !gameTable->at("playtime").is_string() ||
                !gameTable->at("last_launch").is_string()) {
              throw std::runtime_error("Game table has invalid field types");
            }
          }
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
    settings.insert("quit_tray_min", false);
    settings.insert("auto_start", false);
    settings.insert("check_updates", false);
    settings.insert("launcher_icons", false);
    settings.insert("discord_rpc", true);
    settings.insert("accent_color", Themes::ACCENT_COLOR);
    settings.insert("custom_titlebar", true);

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
    gameTable.insert("playtime", toml::value("00:00:00"));
    gameTable.insert("last_launch", toml::value(""));

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
