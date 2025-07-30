#include "parse_acf.h"

#include <fstream>

SteamAppManifest parseAcfFile(const std::string &filepath) {
  SteamAppManifest manifest;
  std::ifstream file(filepath);
  if (!file.is_open()) return manifest;

  std::string line;
  while (std::getline(file, line)) {
    std::string key, value;

    auto firstQuote = line.find('"');
    if (firstQuote == std::string::npos) continue;
    auto secondQuote = line.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) continue;
    key = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);

    auto thirdQuote = line.find('"', secondQuote + 1);
    if (thirdQuote == std::string::npos) continue;
    auto fourthQuote = line.find('"', thirdQuote + 1);
    if (fourthQuote == std::string::npos) continue;
    value = line.substr(thirdQuote + 1, fourthQuote - thirdQuote - 1);

    if (key == "appid") {
      manifest.appid = std::stoi(value);
    } else if (key == "name") {
      manifest.name = value;
    } else if (key == "installdir") {
      manifest.installdir = value;
    } else if (key == "SizeOnDisk") {
      try {
        manifest.sizeOnDisk = std::stoll(value);
      } catch (...) {
        manifest.sizeOnDisk = 0;
      }
    }
  }

  return manifest;
}
