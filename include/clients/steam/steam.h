#pragma once
#include <clients/client.h>

#include <string>
#include <vector>

class Steam : public Client {
 public:
  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
};
