#pragma once

#include <clients/client.h>

#include <string>
#include <vector>

class RockstarGames : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  static std::string getLocation();
  ClientType getType() const override;
};
