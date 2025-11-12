#pragma once
#include <clients/client.h>

#include <string>
#include <vector>

class Steam : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  static std::string getLocation();
  std::string getName() const override;
  ClientType getType() const override;

 private:
  std::vector<std::string> getSteamLibraryFolders() const;
};
