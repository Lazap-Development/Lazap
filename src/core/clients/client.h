#pragma once
#include <cstdint>
#include <string>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
#endif

struct Game {
  std::string name;
  std::string installPath;
  std::string bannerUrl;
  std::string version;
  std::int64_t sizeOnDisk;
  std::string launchArgs;
  std::int32_t appId;
  std::string launcher;
};

class Client {
public:
  virtual std::vector<Game> getInstalledGames() = 0;
  virtual std::string getName() const = 0;
};
