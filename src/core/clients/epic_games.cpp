#include <clients/epic_games.h>

#include <filesystem>
#include <fstream>
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "clients/client.h"

#ifdef _WIN32
#include <windows.h>
#endif

struct EpicGamesItem {
  std::string displayName;
  std::string installLocation;
  std::string launchExecutable;
  std::string appVersionString;
  std::string catalogNamespace;
  std::string catalogItemId;
  std::string appName;
  int64_t installSize{};
};

template <>
struct glz::meta<EpicGamesItem> {
  using T = EpicGamesItem;
  static constexpr auto value =
      object("DisplayName", &T::displayName, "InstallLocation",
             &T::installLocation, "LaunchExecutable", &T::launchExecutable,
             "AppVersionString", &T::appVersionString, "CatalogNamespace",
             &T::catalogNamespace, "CatalogItemId", &T::catalogItemId,
             "AppName", &T::appName, "InstallSize", &T::installSize);
};

static std::string makeAppKey(const EpicGamesItem& item) {
  return item.catalogNamespace + ":" + item.catalogItemId + ":" + item.appName;
}

static std::optional<EpicGamesItem> parseManifest(
    const std::filesystem::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Warning: Could not open manifest file: " << path << std::endl;
    return std::nullopt;
  }

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  EpicGamesItem item;

  if (auto result =
          glz::read<glz::opts{.error_on_unknown_keys = false}>(item, content)) {
    std::cerr << "Warning: Failed to parse JSON in " << path << ": "
              << glz::format_error(result, content) << std::endl;
    return std::nullopt;
  }

  if (!std::filesystem::exists(item.installLocation)) {
    std::cerr << "Warning: Install path not found: " << item.installLocation
              << std::endl;
    return std::nullopt;
  }

  return item;
}

std::string EpicGames::getLocation() {
#ifdef _WIN32
  constexpr auto defaultPath =
      R"(C:\ProgramData\Epic\EpicGamesLauncher\Data\Manifests)";
  return std::filesystem::exists(defaultPath) ? defaultPath : "";
#elif __APPLE__
  if (const char* home = getenv("HOME")) {
    auto path =
        std::string(home) +
        "/Library/Application Support/Epic/EpicGamesLauncher/Data/Manifests";
    return std::filesystem::exists(path) ? path : "";
  }
#else  // Linux
  if (const char* home = getenv("HOME")) {
    const std::vector<std::string> possiblePaths = {
        std::string(home) +
            "/.config/heroic/legendaryConfig/legendary/installed.json",
        std::string(home) +
            "/.var/app/com.heroicgameslauncher.hgl/config/heroic/"
            "legendaryConfig/legendary/installed.json"};
    for (const auto& path : possiblePaths) {
      if (std::filesystem::exists(path)) {
        return std::filesystem::path(path).parent_path().string();
      }
    }
  }
  return "";
#endif
}

std::vector<Game> EpicGames::getInstalledGames() {
  std::vector<Game> games;
  std::unordered_set<size_t> seenAppIds;

  const std::string manifestsPath = getLocation();
  if (manifestsPath.empty() || !std::filesystem::is_directory(manifestsPath)) {
    std::cerr << "Error: Epic Games manifests directory not found.\n";
    return games;
  }

  for (const auto& entry : std::filesystem::directory_iterator(manifestsPath)) {
    if (entry.path().extension() != ".item") continue;

    if (auto item = parseManifest(entry.path())) {
      Game game;
      game.name = item->displayName;
      game.clientType = ClientType::EpicGames;
      game.installPath = "";
      game.executable = "com.epicgames.launcher://apps/" + item->appName +
                        +"?action=launch&silent=true";
      game.bannerUrl = "";
      game.version = item->appVersionString;
      game.sizeOnDisk = item->installSize;
      game.appId = std::hash<std::string>{}(makeAppKey(*item));

      if (seenAppIds.insert(game.appId).second) {
        games.push_back(std::move(game));
      }
    }
  }

  std::cout << "Found " << games.size() << " installed Epic Games.\n";
  return games;
}

std::string EpicGames::getName() const { return "Epic Games"; }

ClientType EpicGames::getType() const { return ClientType::EpicGames; }
