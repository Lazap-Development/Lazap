#pragma once
#include "client.h"
#include <string>
#include <vector>

class Steam : public Client {
public:
  Steam();
  ~Steam() override = default;

  std::vector<Game> getInstalledGames() override;
  std::string getName() const override;
};
