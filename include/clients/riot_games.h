#pragma once

#include <clients/client.h>

#include <string>
#include <vector>

class RiotGames : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
  static std::string getLocation();

 private:
  static Game parseGameObject(const std::string& launcherPath,
                              const std::string& gameName);
};
