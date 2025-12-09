#include "utils/banner_manager.h"

#include <cpr/cpr.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "clients/client.h"
#include "storage/storage_config.h"
#include "utils/fnv1a.h"

namespace fs = std::filesystem;

std::string BannerManager::getBanner(ClientType client,
                                     const std::string& displayName,
                                     const std::string& gameId) {
  switch (client) {
    case ClientType::Steam:
      return fetchBanner("https://cdn.akamai.steamstatic.com/steam/apps/" +
                             gameId + "/library_600x900.jpg",
                         displayName);
    case ClientType::EpicGames:
    case ClientType::UbisoftConnect:
    case ClientType::RiotGames:
      return rawgFetchBanner(displayName);
    case ClientType::RockstarGames:
      return fetchBanner(
          "https://media-rockstargames-com.akamaized.net/rockstargames-newsite/"
          "img/global/games/fob/640/" +
              gameId + ".jpg",
          displayName);
    default:
      return "";
  }
}

std::string BannerManager::fetchBanner(const std::string& url,
                                       const std::string& displayName) {
  std::string bannersDir = StorageConfig::STORAGE_DIR + "/cache/games/banners";
  fs::create_directories(bannersDir);

  std::string displayHash =
      std::to_string(fnv1a::hash(displayName.c_str(), displayName.length()));
  std::string filePath = bannersDir + "/" + displayHash + ".png";

  if (fs::exists(filePath)) {
    return filePath;
  }

  cpr::Response res = cpr::Get(cpr::Url{url});

  if (res.status_code != 200) {
    std::cerr << "[Banner MGR] HTTP error: " << res.status_code << std::endl;
    return "";
  }

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "[Banner MGR] Failed to create file: " << filePath
              << std::endl;
    return "";
  }

  outFile.write(res.text.data(), res.text.size());
  outFile.close();

  return filePath;
}

std::string BannerManager::rawgFetchBanner(const std::string& displayName) {
  const char* raw = std::getenv("RAWG_API_KEY");
  if (!raw) {
    std::cerr
        << "[Banner MGR] WARN: RAWG_API_KEY environment variable is not set!"
        << std::endl;
    return "";
  }

  std::string apiKey{raw};

  std::string bannersDir = StorageConfig::STORAGE_DIR + "/cache/games/banners";
  fs::create_directories(bannersDir);

  std::string displayHash =
      std::to_string(fnv1a::hash(displayName.c_str(), displayName.length()));
  std::string filePath = bannersDir + "/" + displayHash + ".png";

  if (fs::exists(filePath)) {
    return filePath;
  }

  std::string searchTerm =
      replaceAll(replaceAll(displayName, " ", "-"), "_", "-");
  std::string apiUrl =
      "https://api.rawg.io/api/"
      "games?key=" +
      std::string(apiKey) + "&search=" + searchTerm +
      "&search_exact&search_precise&stores=11";

  cpr::Response res = cpr::Get(cpr::Url{apiUrl});

  if (res.status_code != 200) {
    std::cerr << "HTTP error: " << res.status_code << std::endl;
    return "";
  }

  std::string backgroundImageUrl = parseRawgResponse(res.text);
  if (backgroundImageUrl.empty()) {
    return "";
  }

  cpr::Response imgRes = cpr::Get(cpr::Url{backgroundImageUrl});

  if (imgRes.status_code != 200) {
    std::cerr << "[Banner MGR] HTTP error: " << imgRes.status_code << std::endl;
    return "";
  }

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "[Banner MGR] Failed to create file: " << filePath
              << std::endl;
    return "";
  }

  outFile.write(imgRes.text.data(), imgRes.text.size());
  outFile.close();

  return filePath;
}

std::string BannerManager::replaceAll(const std::string& str,
                                      const std::string& from,
                                      const std::string& to) {
  std::string result = str;
  size_t pos = 0;
  while ((pos = result.find(from, pos)) != std::string::npos) {
    result.replace(pos, from.length(), to);
    pos += to.length();
  }
  return result;
}

std::string BannerManager::parseRawgResponse(const std::string& json) {
  size_t resultsPos = json.find("\"results\":[");
  if (resultsPos == std::string::npos) {
    return "";
  }

  size_t startPos = json.find("\"background_image\":\"", resultsPos);
  if (startPos == std::string::npos) {
    return "";
  }

  startPos += 18;
  size_t endPos = json.find("\"", startPos);
  if (endPos == std::string::npos) {
    return "";
  }

  return json.substr(startPos, endPos - startPos);
}
