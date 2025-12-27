#pragma once
#include <clients/client.h>

#include <string>
#include <vector>

class UbisoftConnect : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  static std::string getLocation();
  ClientType getType() const override;
};
