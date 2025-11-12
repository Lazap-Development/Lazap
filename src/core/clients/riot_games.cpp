#include <clients/riot_games.h>

#include <filesystem>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "clients/client.h"
#include "glaze/glaze.hpp"

namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
static std::string runCommand(const std::string& command) {
  STARTUPINFOA si = {sizeof(si)};
  PROCESS_INFORMATION pi;
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;

  HANDLE hRead, hWrite;
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};

  if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return "";

  si.hStdOutput = hWrite;
  si.hStdError = hWrite;

  std::string cmdLine = "cmd.exe /C " + command;
  if (!CreateProcessA(nullptr, cmdLine.data(), nullptr, nullptr, TRUE,
                      CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
    CloseHandle(hRead);
    CloseHandle(hWrite);
    return "";
  }

  CloseHandle(hWrite);
  std::string output;
  char buffer[4096];
  DWORD bytesRead;

  while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) &&
         bytesRead > 0) {
    buffer[bytesRead] = '\0';
    output += buffer;
  }

  CloseHandle(hRead);
  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return output;
}
#endif

// Remove the last two path parts (to get Riot Games directory)
static std::string getRiotGamesLocation(const std::string& launcherPath) {
  std::vector<std::string> parts;
  std::stringstream ss(launcherPath);
  std::string segment;

  while (std::getline(ss, segment, '\\')) parts.push_back(segment);

  if (parts.size() <= 2) return launcherPath;

  std::ostringstream os;
  for (size_t i = 0; i < parts.size() - 2; ++i) {
    if (i > 0) os << "\\";
    os << parts[i];
  }
  return os.str();
}

std::vector<Game> RiotGames::getInstalledGames() {
  std::vector<Game> games;

#if defined(_WIN32)
  std::string output =
      runCommand("Reg Query \"HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon\"");

  if (output.empty()) {
    std::cerr << "Warning: No Riot Client found in registry." << std::endl;
    return games;
  }

  // Extract path between quotes
  std::smatch match;
  std::regex re("\"([^\"]+)\"");
  std::string launcherPath;
  if (std::regex_search(output, match, re)) {
    launcherPath = match[1].str();
  } else {
    std::cerr << "Warning: Failed to extract Riot Client path from registry."
              << std::endl;
    return games;
  }

  std::string basePath = getRiotGamesLocation(launcherPath);
  if (!fs::exists(basePath)) {
    std::cerr << "Warning: Riot base directory not found: " << basePath
              << std::endl;
    return games;
  }

  for (const auto& entry : fs::directory_iterator(basePath)) {
    if (!entry.is_directory()) continue;
    std::string gameName = entry.path().filename().string();
    if (gameName == "Riot Client") continue;

    games.push_back(parseGameObject(launcherPath, gameName));
  }
#endif
  std::cout << "Found " << games.size() << " Riot games." << std::endl;
  return games;
}

Game RiotGames::parseGameObject(const std::string& launcherPath,
                                const std::string& gameName) {
  std::string correctArg;
  if (gameName == "VALORANT")
    correctArg = "valorant";
  else if (gameName == "League of Legends")
    correctArg = "league_of_legends";
  else if (gameName == "LoR")
    correctArg = "bacon";
  else
    correctArg = "";

  std::string executable = "RiotClientServices.exe";
  std::string location =
      launcherPath.substr(0, launcherPath.size() > 22 ? launcherPath.size() - 22
                                                      : launcherPath.size());

  std::vector<std::string> args = {"launch-product=" + correctArg,
                                   "--launch-patchline=live"};

  Game g;
  g.appId = std::hash<std::string>{}("riot:" + gameName);
  g.clientType = ClientType::RiotGames;
  g.installPath = location;
  g.name = gameName;
  g.executable = executable;
  g.version = "unknown";
  g.sizeOnDisk = 0;
  g.bannerUrl = "";

  return g;
}

std::string RiotGames::getName() const { return "Riot Games"; }
