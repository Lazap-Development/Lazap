#include <clients/epic/epic.h>

#include <filesystem>
#include <fstream>
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "clients/client.h"

#ifdef _WIN32
#include <windows.h>
#endif

struct EpicGamesItem {
  int formatVersion = 0;

  bool bIsIncompleteInstall = false;
  std::string launchCommand;
  std::string launchExecutable;
  std::string manifestLocation;
  std::string manifestHash;
  bool bIsApplication = false;
  bool bIsExecutable = false;
  bool bIsManaged = false;
  bool bNeedsValidation = false;
  bool bRequiresAuth = false;
  bool bAllowMultipleInstances = false;
  bool bCanRunOffline = false;
  bool bAllowUriCmdArgs = false;
  bool bLaunchElevated = false;
  std::vector<std::string> baseURLs;
  std::string buildLabel;
  std::vector<std::string> appCategories;
  std::vector<std::string> chunkDbs;
  std::vector<std::string> compatibleApps;
  std::string displayName;
  std::string installationGuid;
  std::string installLocation;
  std::string installSessionId;
  std::vector<std::string> installTags;
  std::vector<std::string> installComponents;
  std::string hostInstallationGuid;
  std::vector<std::string> prereqIds;
  std::string prereqSHA1Hash;
  std::string lastPrereqSucceededSHA1Hash;
  std::string stagingLocation;
  std::string technicalType;
  std::string vaultThumbnailUrl;
  std::string vaultTitleText;
  int64_t installSize = 0;
  std::string mainWindowProcessName;
  std::vector<std::string> processNames;
  std::vector<std::string> backgroundProcessNames;
  std::vector<std::string> ignoredProcessNames;
  std::vector<std::string> dlcProcessNames;
  std::string mandatoryAppFolderName;
  std::string ownershipToken;
  int sidecarConfigRevision = 0;
  std::string catalogNamespace;
  std::string catalogItemId;
  std::string appName;
  std::string appVersionString;
  std::string mainGameCatalogNamespace;
  std::string mainGameCatalogItemId;
  std::string mainGameAppName;
  std::vector<std::string> allowedUriEnvVars;
  int preloadState = 0;
};

template <>
struct glz::meta<EpicGamesItem> {
  using T = EpicGamesItem;
  static constexpr auto value = object(
      "FormatVersion", &T::formatVersion, "bIsIncompleteInstall",
      &T::bIsIncompleteInstall, "LaunchCommand", &T::launchCommand,
      "LaunchExecutable", &T::launchExecutable, "ManifestLocation",
      &T::manifestLocation, "ManifestHash", &T::manifestHash, "bIsApplication",
      &T::bIsApplication, "bIsExecutable", &T::bIsExecutable, "bIsManaged",
      &T::bIsManaged, "bNeedsValidation", &T::bNeedsValidation, "bRequiresAuth",
      &T::bRequiresAuth, "bAllowMultipleInstances", &T::bAllowMultipleInstances,
      "bCanRunOffline", &T::bCanRunOffline, "bAllowUriCmdArgs",
      &T::bAllowUriCmdArgs, "bLaunchElevated", &T::bLaunchElevated, "BaseURLs",
      &T::baseURLs, "BuildLabel", &T::buildLabel, "AppCategories",
      &T::appCategories, "ChunkDbs", &T::chunkDbs, "CompatibleApps",
      &T::compatibleApps, "DisplayName", &T::displayName, "InstallationGuid",
      &T::installationGuid, "InstallLocation", &T::installLocation,
      "InstallSessionId", &T::installSessionId, "InstallTags", &T::installTags,
      "InstallComponents", &T::installComponents, "HostInstallationGuid",
      &T::hostInstallationGuid, "PrereqIds", &T::prereqIds, "PrereqSHA1Hash",
      &T::prereqSHA1Hash, "LastPrereqSucceededSHA1Hash",
      &T::lastPrereqSucceededSHA1Hash, "StagingLocation", &T::stagingLocation,
      "TechnicalType", &T::technicalType, "VaultThumbnailUrl",
      &T::vaultThumbnailUrl, "VaultTitleText", &T::vaultTitleText,
      "InstallSize", &T::installSize, "MainWindowProcessName",
      &T::mainWindowProcessName, "ProcessNames", &T::processNames,
      "BackgroundProcessNames", &T::backgroundProcessNames,
      "IgnoredProcessNames", &T::ignoredProcessNames, "DlcProcessNames",
      &T::dlcProcessNames, "MandatoryAppFolderName", &T::mandatoryAppFolderName,
      "OwnershipToken", &T::ownershipToken, "SidecarConfigRevision",
      &T::sidecarConfigRevision, "CatalogNamespace", &T::catalogNamespace,
      "CatalogItemId", &T::catalogItemId, "AppName", &T::appName,
      "AppVersionString", &T::appVersionString, "MainGameCatalogNamespace",
      &T::mainGameCatalogNamespace, "MainGameCatalogItemId",
      &T::mainGameCatalogItemId, "MainGameAppName", &T::mainGameAppName,
      "AllowedUriEnvVars", &T::allowedUriEnvVars, "PreloadState",
      &T::preloadState);
};

std::string EpicGames::getLocation() {
  std::string epicInstallPath;

#ifdef _WIN32
  epicInstallPath = "C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests";
#elif __APPLE__
  const char* home = getenv("HOME");
  if (home) {
    epicInstallPath =
        std::string(home) +
        "/Library/Application Support/Epic/EpicGamesLauncher/Data/Manifests";
  }
#else  // Linux
  const char* home = getenv("HOME");
  if (home) {
    std::vector<std::string> possiblePaths = {
        std::string(home) +
            "/.config/heroic/legendaryConfig/legendary/installed.json",
        std::string(home) +
            "/.var/app/com.heroicgameslauncher.hgl/config/heroic/"
            "legendaryConfig/legendary/installed.json"};

    for (const auto& path : possiblePaths) {
      if (std::filesystem::exists(path)) {
        epicInstallPath = std::filesystem::path(path).parent_path().string();
        break;
      }
    }
  }
#endif

  return epicInstallPath;
}

std::vector<Game> EpicGames::getInstalledGames() {
  std::vector<Game> games;
  std::string manifestsPath = getLocation();

  if (manifestsPath.empty()) {
    std::cerr << "Error: Could not find Epic Games manifests directory"
              << std::endl;
    return games;
  }

  if (!std::filesystem::exists(manifestsPath) ||
      !std::filesystem::is_directory(manifestsPath)) {
    std::cerr << "Warning: Epic Games manifests directory not found: "
              << manifestsPath << std::endl;
    return games;
  }

  try {
    for (const auto& entry :
         std::filesystem::directory_iterator(manifestsPath)) {
      if (entry.path().extension() == ".item") {
        try {
          std::ifstream file(entry.path());
          if (!file.is_open()) {
            std::cerr << "Warning: Could not open manifest file: "
                      << entry.path() << std::endl;
            continue;
          }

          std::string fileContent((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());

          EpicGamesItem epicItem;
          auto result = glz::read_json(epicItem, fileContent);

          if (result) {
            std::cerr << "Warning: Failed to parse JSON in file "
                      << entry.path() << ": "
                      << glz::format_error(result, fileContent) << std::endl;
            continue;
          }

          if (!std::filesystem::exists(epicItem.installLocation)) {
            std::cerr << "Warning: Game installation path does not exist: "
                      << epicItem.installLocation << std::endl;
            continue;
          }

          Game game;
          game.name = epicItem.displayName;
          game.clientType = ClientType::EpicGames;
          game.installPath = epicItem.installLocation;
          game.executable = epicItem.launchExecutable;
          game.bannerUrl = "";  // To Do: Fetch banner to show in UI
          game.version = epicItem.appVersionString;
          game.sizeOnDisk = epicItem.installSize;

          // Create a unique Identifier
          game.appId = std::hash<std::string>{}(epicItem.catalogNamespace +
                                                ":" + epicItem.catalogItemId +
                                                ":" + epicItem.appName);

          // Check for duplicates
          bool duplicate = false;
          for (const auto& g : games) {
            if (g.appId == game.appId) {
              duplicate = true;
              break;
            }
          }

          if (!duplicate) {
            games.push_back(std::move(game));
          }

        } catch (const std::exception& e) {
          std::cerr << "Warning: Could not parse Epic Games manifest file "
                    << entry.path() << ": " << e.what() << std::endl;
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error reading Epic Games manifests directory: " << e.what()
              << std::endl;
  }

  std::cout << "Found " << games.size() << " installed Epic Games" << std::endl;
  return games;
}

std::string EpicGames::getName() const { return "Epic Games"; }