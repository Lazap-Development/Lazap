#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Client;

enum class ClientType { Steam };

struct Game {
  std::string name;
  std::string installPath;
  std::string bannerUrl;
  std::string version;
  std::int64_t sizeOnDisk;
  std::string executable;
  std::int32_t appId;
  ClientType client;
};

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
// You can use this macro for platform-specific code
#endif

class Client {
 public:
  using Type = ClientType;

  virtual ~Client() = default;
  virtual std::vector<Game> getInstalledGames() = 0;
  virtual std::string getName() const = 0;
};
