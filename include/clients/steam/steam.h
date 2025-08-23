#pragma once
#include <clients/client.h>

#include <string>
#include <vector>

class Steam : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  static std::string getLocation();
  std::vector<std::string> getSteamLibraryFolders() const;
  std::string getName() const override;
};
