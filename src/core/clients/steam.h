#pragma once
#include <string>
#include <vector>

#include "client.h"

#if defined(__linux__) || defined(__APPLE__)
#define LINUX_OR_APPLE
#endif

class Steam : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
};
