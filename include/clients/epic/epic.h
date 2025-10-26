#pragma once
#include <clients/client.h>

#include <string>
#include <vector>

class EpicGames : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
  static std::string getLocation();
};