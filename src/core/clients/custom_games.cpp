#include <clients/custom_games.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

#include "clients/client.h"
#include "storage/storage.h"
#include "utils/fnv1a.h"

namespace fs = std::filesystem;

CustomGames::CustomGames(Storage& storage) : storage_(storage) {}

static std::string extractExecutableName(const std::string& fullPath) {
  fs::path path(fullPath);
  return path.filename().string();
}

static std::string extractLocation(const std::string& fullPath) {
  fs::path path(fullPath);
  return path.parent_path().string();
}

std::vector<Game> CustomGames::getInstalledGames() {
  std::vector<Game> games;

  try {
    toml::table config = storage_.loadTOML();

    if (!config.contains("custom_games")) {
      return games;
    }

    auto customGamesTable = config["custom_games"].as_table();
    if (!customGamesTable) {
      return games;
    }
    for (const auto& [gameIdStr, gameValue] : *customGamesTable) {
      if (!gameValue.is_table()) {
        continue;
      }

      auto gameTable = gameValue.as_table();

      auto gameName = gameTable->at("name").value<std::string>();
      auto executable = gameTable->at("executable").value<std::string>();
      auto location = gameTable->at("location").value<std::string>();
      auto bannerPath = gameTable->at("banner_path").value<std::string>();

      if (!gameName || !executable || !location) {
        std::cerr << "Warning: Incomplete custom game entry: " << gameIdStr
                  << std::endl;
        continue;
      }

      fs::path execPath = fs::path(*location) / *executable;

      Game game;
      game.name = *gameName;
      game.clientType = ClientType::CustomGames;
      game.installPath = *location;
      game.executable = execPath.string();
      game.bannerUrl = *bannerPath;
      game.version = "";

      try {
        game.appId = std::stoull(std::string(gameIdStr));
      } catch (...) {
        game.appId = fnv1a::hash(gameName->c_str(), gameName->length());
      }
      if (fs::exists(game.executable)) {
        games.push_back(std::move(game));
      } else {
        std::cerr << "Warning: Custom game executable not found: "
                  << game.executable << std::endl;
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Error loading custom games: " << e.what() << std::endl;
  }

  std::cout << "Found " << games.size() << " custom games.\n";
  return games;
}

bool CustomGames::addCustomGame(const std::string& location,
                                const std::string& gameName,
                                const std::string& bannerPath) {
  if (!fs::exists(location)) {
    std::cerr << "Error: Executable path does not exist: " << location
              << std::endl;
    return false;
  }

  try {
    size_t gameId = fnv1a::hash(gameName.c_str(), gameName.length());
    std::string gameIdStr = std::to_string(gameId);

    storage_.updateTOML([&](toml::table& config) {
      if (!config.contains("custom_games"))
        config.insert("custom_games", toml::table{});
      auto customGamesTable = config["custom_games"].as_table();

      toml::table customEntry;
      customEntry.insert("name", gameName);
      customEntry.insert("executable",
                         toml::value(extractExecutableName(location)));
      customEntry.insert("location", toml::value(extractLocation(location)));
      customEntry.insert("banner_path", toml::value(bannerPath));
      customGamesTable->insert(gameIdStr, customEntry);

      if (!config.contains("games")) {
        config.insert("games", toml::table{});
      }
      auto gamesTable = config["games"].as_table();
      if (gamesTable && !gamesTable->contains(gameIdStr)) {
        toml::table meta;
        meta.insert("name", gameName);
        meta.insert("favourite", false);
        meta.insert("playtime", "00:00:00");
        meta.insert("last_launch", "");
        gamesTable->insert(gameIdStr, meta);
      }
    });

    std::cout << "Successfully added custom game: " << gameName << std::endl;
    return true;
  } catch (const std::exception& e) {
    std::cerr << "Error adding custom game: " << e.what() << std::endl;
    return false;
  }
}

ClientType CustomGames::getType() const { return ClientType::CustomGames; }
