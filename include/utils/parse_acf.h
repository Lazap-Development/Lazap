#pragma once

#include <string>

struct SteamAppManifest {
  int appid = 0;
  std::string name;
  std::string installdir;
  long long sizeOnDisk = 0;
};

SteamAppManifest parseAcfFile(const std::string &filepath);
