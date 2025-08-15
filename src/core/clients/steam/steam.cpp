#include <clients/steam/steam.h>
#include <utils/parse_acf.h>
#include <utils/vdf_parser.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#ifdef _WIN32
#include <windows.h>

#include <iostream>
#include <ostream>
#endif

// Blacklisted App IDs (Steam tools, redistributables, etc.)
// TODO: Somehow filter tools out of games
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

std::vector<std::string> getSteamLibraryFolders() {
  std::vector<std::string> paths;
  std::string steamInstallPath;
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
      steamInstallPath = std::string(installPath);
      libraryVDF = steamInstallPath + "\\steamapps\\libraryfolders.vdf";
    }
    RegCloseKey(hKey);
  }
#elif __APPLE__
  const char *home = getenv("HOME");
  if (home) {
    steamInstallPath = std::string(home) + "/Library/Application Support/Steam";
    libraryVDF = steamInstallPath + "/steamapps/libraryfolders.vdf";
  }
#else  // Linux
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
        libraryVDF = testVDF;
        break;
      }
    }
  }
#endif

  if (!libraryVDF.empty() && std::filesystem::exists(libraryVDF)) {
    paths = parseLibraryFoldersVDF(libraryVDF);
  } else {
    std::cerr << "Warning: Could not find Steam libraryfolders.vdf"
              << std::endl;
  }

  return paths;
}

std::string findExecutableForOS(const tyti::vdf::object &root,
                                const std::string &app_id,
                                const std::string &desired_os) {
  std::string appKey = "app_" + app_id;

  auto appIt = root.childs.find(appKey);
  if (appIt == root.childs.end() || !appIt->second) return "";

  auto &appNode = appIt->second;
  auto configIt = appNode->childs.find("config");
  if (configIt == appNode->childs.end() || !configIt->second) return "";

  auto &configNode = configIt->second;
  auto launchIt = configNode->childs.find("launch");
  if (launchIt == configNode->childs.end() || !launchIt->second) return "";

  auto &launchNode = launchIt->second;

  for (const auto &[key, child] : launchNode->childs) {
    if (!child) continue;

    auto execIt = child->attribs.find("executable");
    if (execIt == child->attribs.end()) continue;

    auto configChildIt = child->childs.find("config");
    if (configChildIt == child->childs.end() || !configChildIt->second) {
      // No config child, so return executable immediately
      return execIt->second;
    }

    auto &configChild = configChildIt->second;
    auto osIt = configChild->attribs.find("oslist");
    if (osIt == configChild->attribs.end()) {
      // config exists but no oslist, still return executable immediately
      return execIt->second;
    }

    // config and oslist exist, return executable only if oslist matches
    if (osIt->second == desired_os) {
      return execIt->second;
    }
  }

  return "";
}

std::string normalizeExecutablePath(const std::string &path) {
#ifdef _WIN32
  return path;
#else
  std::string normalized = path;
  std::replace(normalized.begin(), normalized.end(), '\\', '/');
  return normalized;
#endif
}

std::string makeExecutablePath(const std::string &installPath,
                               const std::string &execPath) {
  if (execPath.empty()) return "";

  std::string normalizedExecPath = normalizeExecutablePath(execPath);
  std::filesystem::path fullPath =
      std::filesystem::path(installPath) / normalizedExecPath;
  return fullPath.string();
}

std::vector<Game> Steam::getInstalledGames() {
  std::vector<Game> games;
  auto steamPaths = getSteamLibraryFolders();

  if (steamPaths.empty()) {
    std::cerr << "Error: No Steam library folders found" << std::endl;
    return games;
  }

  std::ifstream ifs("appinfo_text.vdf");
  tyti::vdf::object root = tyti::vdf::read(ifs);

  int result = std::system("./SteamAppInfo");
  if (result != -1) {
  } else {
    std::ifstream ifs("appinfo_text.vdf");
    tyti::vdf::object root = tyti::vdf::read(ifs);
  }

  for (const auto &steamPath : steamPaths) {
    std::filesystem::path steamAppsPath =
        std::filesystem::path(steamPath) / "steamapps";

    if (!std::filesystem::exists(steamAppsPath) ||
        !std::filesystem::is_directory(steamAppsPath)) {
      std::cerr << "Warning: Steam apps directory not found: " << steamAppsPath
                << std::endl;
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
          game.installPath = installPath.string();
          game.bannerUrl = "";
          game.version = "unknown";
          game.sizeOnDisk = manifest.sizeOnDisk;
          game.appId = manifest.appid;
          game.client = ClientType::Steam;

          std::string current_os;
#if defined(_WIN32) || defined(_WIN64)
          current_os = "windows";
#elif defined(__APPLE__) || defined(__MACH__)
          current_os = "macos";
#elif defined(__linux__)
          current_os = "linux";
#else
          current_os = "unknown";
#endif

          std::string appIdStr = std::to_string(manifest.appid);

          if (current_os == "linux") {
            std::string exec_path =
                findExecutableForOS(root, appIdStr, "linux");
            if (!exec_path.empty()) {
              game.executable = makeExecutablePath(game.installPath, exec_path);
            } else {
              exec_path = findExecutableForOS(root, appIdStr, "windows");
              if (!exec_path.empty()) {
                game.executable =
                    makeExecutablePath(game.installPath, exec_path);
              } else {
                game.executable = "";
              }
            }
          } else {
            std::string exec_path =
                findExecutableForOS(root, appIdStr, current_os);
            if (!exec_path.empty()) {
              game.executable = makeExecutablePath(game.installPath, exec_path);
            } else {
              game.executable = "";
            }
          }

          bool duplicate = false;
          for (const auto &g : games) {
            if (g.appId == game.appId) {
              duplicate = true;
              break;
            }
          }

          std::cout << game.executable << "\n";

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
