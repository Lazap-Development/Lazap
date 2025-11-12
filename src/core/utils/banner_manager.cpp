#include "utils/banner_manager.h"

#include <httplib.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

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
      // case ClientType::RiotGames:
      return rawgFetchBanner(displayName);
    // case ClientType::RockstarGames:
    //  return fetchBanner(
    //      "https://media-rockstargames-com.akamaized.net/rockstargames-newsite/"
    //      "img/global/games/fob/640/" +
    //          gameId + ".jpg",
    //      displayName);
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

  size_t pathStart = url.find('/', 8);
  std::string baseUrl = url.substr(0, pathStart);
  std::string path = url.substr(pathStart);

  httplib::Client cli(baseUrl);
  auto res = cli.Get(path.c_str());

  if (!res || res->status != 200) {
    std::cerr << "HTTP error: "
              << (res ? std::to_string(res->status) : "connection failed")
              << std::endl;
    return "";
  }

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "Failed to create file: " << filePath << std::endl;
    return "";
  }

  outFile.write(res->body.data(), res->body.size());
  outFile.close();

  return filePath;
}

std::string BannerManager::rawgFetchBanner(const std::string& displayName) {
  std::string_view apiKey = std::getenv("RAWG_API_KEY");

  if (apiKey.empty()) {
    std::cerr << "Error: RAWG_API_KEY environment variable is not set!"
              << std::endl;
    return "";
  }

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

  size_t pathStart = apiUrl.find('/', 8);
  std::string baseUrl = apiUrl.substr(0, pathStart);
  std::string path = apiUrl.substr(pathStart);

  httplib::Client cli(baseUrl);
  auto res = cli.Get(path.c_str());

  if (!res || res->status != 200) {
    std::cerr << "HTTP error: "
              << (res ? std::to_string(res->status) : "connection failed")
              << std::endl;
    return "";
  }

  std::string backgroundImageUrl = parseRawgResponse(res->body);
  if (backgroundImageUrl.empty()) {
    return "";
  }

  httplib::Client imgCli(backgroundImageUrl);
  auto imgRes = imgCli.Get(backgroundImageUrl.c_str());

  if (!imgRes || imgRes->status != 200) {
    std::cerr << "HTTP error: "
              << (imgRes ? std::to_string(imgRes->status) : "connection failed")
              << std::endl;
    return "";
  }

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "Failed to create file: " << filePath << std::endl;
    return "";
  }

  outFile.write(imgRes->body.data(), imgRes->body.size());
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
