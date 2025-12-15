#pragma once
#include <clients/client.h>
#include <storage/storage.h>

#include <string>
#include <vector>

class CustomGames : public Client {
 public:
  explicit CustomGames(Storage& storage);
  std::vector<Game> getInstalledGames() override;
  bool addCustomGame(const std::string& location,
                     const std::string& bannerPath = "");
  ClientType getType() const override;

 private:
  Storage& storage_;
};