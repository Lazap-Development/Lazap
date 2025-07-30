#include "steam.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../utils/parse_acf.h"
#include "vdf_parser.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

const std::unordered_set<int> BLACKLIST_APPID = {
    228980,  231350,  1493710, 1391110, 1070560, 1826330, 1113280,
    1245040, 1420170, 1580130, 1887720, 1628350, 2348590, 2180100,
};

std::vector<std::string> parseLibraryFoldersVDF(const std::string &filepath) {
  std::vector<std::string> paths;
  std::ifstream file(filepath);
  if (!file.is_open()) return paths;

  tyti::vdf::object root = tyti::vdf::read(file);

  for (const auto &[key, childPtr] : root.childs) {
    auto it = childPtr->attribs.find("path");
    if (it != childPtr->attribs.end()) {
      paths.push_back(it->second);
    }
  }

  return paths;
}

std::vector<std::string> getSteamLibraryFolders() {
  std::vector<std::string> paths;
  std::string libraryVDF;

#ifdef _WIN32
  HKEY hKey;
  const char *keyPath = "SOFTWARE\\WOW6432Node\\Valve\\Steam";
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hKey) ==
      ERROR_SUCCESS) {
    char installPath[MAX_PATH];
    DWORD pathSize = sizeof(installPath);
    DWORD type = REG_SZ;

    if (RegQueryValueExA(hKey, "InstallPath", nullptr, &type,
                         (LPBYTE)installPath, &pathSize) == ERROR_SUCCESS) {
      libraryVDF = std::string(installPath) + "\\steamapps\\libraryfolders.vdf";
    }

    RegCloseKey(hKey);
  }
#elif __APPLE__
  libraryVDF =
      std::string(getenv("HOME")) +
      "/Library/Application Support/Steam/steamapps/libraryfolders.vdf";
#else
  libraryVDF = std::string(getenv("HOME")) +
               "/.steam/steam/steamapps/libraryfolders.vdf";
#endif

  if (!libraryVDF.empty() && std::filesystem::exists(libraryVDF)) {
    paths = parseLibraryFoldersVDF(libraryVDF);
  }
  return paths;
}

std::vector<Game> Steam::getInstalledGames() {
  std::vector<Game> games;
  auto steamPaths = getSteamLibraryFolders();

  for (const auto &steamPath : steamPaths) {
    std::filesystem::path steamAppsPath =
        std::filesystem::path(steamPath) / "steamapps";

    if (!std::filesystem::exists(steamAppsPath) ||
        !std::filesystem::is_directory(steamAppsPath)) {
      continue;
    }

    for (auto &entry : std::filesystem::directory_iterator(steamAppsPath)) {
      if (entry.path().extension() == ".acf") {
        SteamAppManifest manifest = parseAcfFile(entry.path().string());

        if (manifest.appid == 0 || BLACKLIST_APPID.count(manifest.appid) > 0) {
          continue;
        }

        std::filesystem::path installPath =
            std::filesystem::path(steamAppsPath) / "common" /
            manifest.installdir;

        Game game;
        game.name = manifest.name;
        game.installPath = installPath.string();
        game.bannerUrl = "";
        game.version = "unknown";
        game.sizeOnDisk = manifest.sizeOnDisk;
#ifdef _WIN32
        game.launchArgs = "steam://run/" + std::to_string(manifest.appid);
#elif defined(LINUX_OR_APPLE)
        game.launchArgs =
            "-silent -applaunch " + std::to_string(manifest.appid);
#endif
        game.appId = manifest.appid;
        game.launcher = "steam";

        bool duplicate = false;
        for (const auto &g : games) {
          if (g.name == game.name) {
            duplicate = true;
            break;
          }
        }
        if (!duplicate) {
          games.push_back(std::move(game));
        }
      }
    }
  }

  return games;
}

std::string Steam::getName() const { return "Steam"; }
