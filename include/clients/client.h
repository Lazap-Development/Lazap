#pragma once
#include <utils/launch_manager.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Client;

struct Game {
  std::string name;
  std::string installPath;
  std::string bannerUrl;
  std::string version;
  std::int64_t sizeOnDisk;
  std::string executable;
  std::int32_t appId;
  std::unique_ptr<LaunchManager> launchManager;
};

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
// You can use this macro for platform-specific code
#endif

class Client {
 public:
  virtual ~Client() = default;
  virtual std::vector<Game> getInstalledGames() = 0;
  virtual std::string getName() const = 0;
};
