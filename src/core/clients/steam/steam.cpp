#include <clients/steam/appinfo.h>
#include <clients/steam/steam.h>
#include <utils/parse_acf.h>
#include <utils/vdf_parser.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "clients/client.h"

#ifdef _WIN32
#include <windows.h>

#include <iostream>
#include <ostream>
#endif

const std::unordered_set<int> BLACKLIST_APPID = {
    228980,   // Steamworks Common Redistributables
    231350,   // VCRUNTIME
    1493710,  // Proton
    1391110,  // EA Desktop
    1070560,  // Steam Linux Runtime
    1826330,  // Steam Linux Runtime 2.0
    1113280,  // Proton Experimental
    1245040,  // Proton 6.3
    1420170,  // Proton 7.0
    1580130,  // Proton 8.0
    2805730,  // Proton 9.0
    1628350,  // Steam Linux Runtime Container
    2348590,  // Proton Hotfix
    2180100,  // Proton Next
};

std::vector<std::string> parseLibraryFoldersVDF(const std::string &filepath) {
  std::vector<std::string> paths;
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Warning: Could not open libraryfolders.vdf at " << filepath
              << std::endl;
    return paths;
  }

  try {
    tyti::vdf::object root = tyti::vdf::read(file);

    for (const auto &[key, childPtr] : root.childs) {
      auto it = childPtr->attribs.find("path");
      if (it != childPtr->attribs.end()) {
        paths.push_back(it->second);
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error parsing libraryfolders.vdf: " << e.what() << std::endl;
  }

  return paths;
}

std::string Steam::getLocation() {
  std::string steamInstallPath;

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
      steamInstallPath = std::string(installPath);
    }
    RegCloseKey(hKey);
  }
#elif __APPLE__
  const char *home = getenv("HOME");
  if (home) {
    steamInstallPath = std::string(home) + "/Library/Application Support/Steam";
  }
#else
  const char *home = getenv("HOME");
  if (home) {
    std::vector<std::string> possiblePaths = {
        std::string(home) + "/.steam/steam",
        std::string(home) + "/.local/share/Steam",
        std::string(home) + "/.steam/root"};

    for (const auto &path : possiblePaths) {
      std::string testVDF = path + "/steamapps/libraryfolders.vdf";
      if (std::filesystem::exists(testVDF)) {
        steamInstallPath = path;
        break;
      }
    }
  }
#endif

  return steamInstallPath;
}

std::vector<std::string> Steam::getSteamLibraryFolders() const {
  std::vector<std::string> paths;
  std::string steamInstallPath = getLocation();

  if (steamInstallPath.empty()) {
    return paths;
  }

  std::string libraryVDF;
#ifdef _WIN32
  libraryVDF = steamInstallPath + "\\steamapps\\libraryfolders.vdf";
#else
  libraryVDF = steamInstallPath + "/steamapps/libraryfolders.vdf";
#endif

  if (std::filesystem::exists(libraryVDF)) {
    paths = parseLibraryFoldersVDF(libraryVDF);
  } else {
    std::cerr << "Warning: Could not find Steam libraryfolders.vdf"
              << std::endl;
  }

  return paths;
}

std::vector<Game> Steam::getInstalledGames() {
  std::vector<Game> games;
  auto steamPaths = getSteamLibraryFolders();

  if (steamPaths.empty()) {
    std::cerr << "Error: No Steam library folders found" << std::endl;
    return games;
  }

  std::string steamInstallPath = getLocation();
  if (steamInstallPath.empty()) {
    std::cerr << "Error: Could not find Steam installation path" << std::endl;
    return games;
  }

#ifdef _WIN32
  std::string appInfoPath = steamInstallPath + "\\appcache\\appinfo.vdf";
#else
  std::string appInfoPath = steamInstallPath + "/appcache/appinfo.vdf";
#endif

  AppInfoParser parser;
  if (!parser.loadFile(appInfoPath)) {
    std::cerr << "Warning: Could not load appinfo.vdf from " << appInfoPath
              << std::endl;
  }

  for (const auto &steamPath : steamPaths) {
    std::filesystem::path steamAppsPath =
        std::filesystem::path(steamPath) / "steamapps";

    if (!std::filesystem::exists(steamAppsPath) ||
        !std::filesystem::is_directory(steamAppsPath)) {
      std::cerr
          << "Warning: Steam directory not found (may be false-positive): "
          << steamAppsPath << std::endl;
      continue;
    }

    for (auto &entry : std::filesystem::directory_iterator(steamAppsPath)) {
      if (entry.path().extension() == ".acf") {
        try {
          SteamAppManifest manifest = parseAcfFile(entry.path().string());

          if (manifest.appid == 0 ||
              BLACKLIST_APPID.count(manifest.appid) > 0) {
            continue;
          }

          std::filesystem::path installPath =
              steamAppsPath / "common" / manifest.installdir;

          if (!std::filesystem::exists(installPath)) {
            continue;
          }

          Game game;
          game.name = manifest.name;
          game.clientType = ClientType::Steam;
          game.installPath = installPath.string();
          game.bannerUrl = "";
          game.version = "unknown";
          game.sizeOnDisk = manifest.sizeOnDisk;
          game.appId = manifest.appid;

          auto executables = parser.getLaunchConfig(manifest.appid);
          std::string currentOS;
#ifdef _WIN32
          currentOS = "windows";
#elif __linux__
          currentOS = "linux";
#elif __APPLE__
          currentOS = "macos";
#endif

          std::string fallbackExecutable;

          for (const auto &executable : executables) {
            auto osIt = executable.find("oslist");
            auto execIt = executable.find("executable");

            if (execIt == executable.end()) continue;
            if (osIt != executable.end()) {
              if (osIt->second == currentOS) {
                game.executable = execIt->second;
                break;
              }
              if (currentOS == "linux" && osIt->second == "windows" &&
                  fallbackExecutable.empty()) {
                fallbackExecutable = execIt->second;
              }
            } else if (fallbackExecutable.empty()) {
              fallbackExecutable = execIt->second;
            }
          }

          if (game.executable.empty() && !fallbackExecutable.empty()) {
            game.executable = fallbackExecutable;
          }

          bool duplicate = false;
          for (const auto &g : games) {
            if (g.appId == game.appId) {
              duplicate = true;
              break;
            }
          }

          if (!duplicate) {
            games.push_back(std::move(game));
          }
        } catch (const std::exception &e) {
          std::cerr << "Warning: Could not parse ACF file " << entry.path()
                    << ": " << e.what() << std::endl;
        }
      }
    }
  }

  std::cout << "Found " << games.size() << " installed Steam games"
            << std::endl;
  return games;
}

std::string Steam::getName() const { return "Steam"; }
