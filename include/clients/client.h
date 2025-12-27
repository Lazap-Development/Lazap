#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Client;

enum class ClientType {
  Steam,
  EpicGames,
  UbisoftConnect,
  RockstarGames,
  RiotGames,
  CustomGames
};

struct Game {
  std::string name;
  std::string installPath;
  std::string bannerUrl;
  std::string version;
  std::int64_t sizeOnDisk;
  std::string executable;
  ClientType clientType;
  std::int32_t appId;
};

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
#endif

class Client {
 public:
  virtual ~Client() = default;
  virtual std::vector<Game> getInstalledGames() = 0;
  virtual ClientType getType() const = 0;
};
