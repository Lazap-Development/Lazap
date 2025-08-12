#pragma once
#include <string>
#include <vector>

#include "client.h"

class Steam : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
};
