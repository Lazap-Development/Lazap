#pragma once
#include <cstdint>
#include <string>
#include <vector>

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
  virtual ~Client() = default;

  virtual std::vector<Game> getInstalledGames() = 0;
  virtual std::string getName() const = 0;
};
