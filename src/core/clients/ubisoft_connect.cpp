#include <clients/ubisoft_connect.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "clients/client.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;
#ifdef _WIN32
static std::string queryRegistry(const std::string& command) {
  STARTUPINFOA si = {sizeof(si)};
  PROCESS_INFORMATION pi;
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;

  HANDLE hReadPipe, hWritePipe;
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

  if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) return "";

  si.hStdOutput = hWritePipe;
  si.hStdError = hWritePipe;

  std::string cmdLine = "cmd.exe /C " + command;
  if (!CreateProcessA(NULL, &cmdLine[0], NULL, NULL, TRUE, CREATE_NO_WINDOW,
                      NULL, NULL, &si, &pi)) {
    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);
    return "";
  }

  CloseHandle(hWritePipe);
  std::string output;
  char buffer[4096];
  DWORD bytesRead;

  while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) &&
         bytesRead > 0) {
    buffer[bytesRead] = '\0';
    output += buffer;
  }

  CloseHandle(hReadPipe);
  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return output;
}
#endif

std::string UbisoftConnect::getLocation() {
#ifdef _WIN32
  const char* regPath =
#ifdef _WIN64
      "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Ubisoft\\Launcher";
#else
      "HKEY_LOCAL_MACHINE\\SOFTWARE\\Ubisoft\\Launcher";
#endif

  std::string output =
      queryRegistry("Reg query \"" + std::string(regPath) + "\"");
  if (output.empty()) return "";

  std::regex re(R"(InstallDir\s+REG_SZ\s+([^\r\n]+))");
  std::smatch match;
  if (std::regex_search(output, match, re)) {
    return match[1].str();
  }
#endif
  return "";
}

std::vector<Game> UbisoftConnect::getInstalledGames() {
  std::vector<Game> games;
#ifdef _WIN32
  const char* regPath =
#ifdef _WIN64
      "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Ubisoft\\Launcher\\Installs";
#else
      "HKEY_LOCAL_MACHINE\\SOFTWARE\\Ubisoft\\Launcher\\Installs";
#endif

  std::string output =
      queryRegistry("Reg query \"" + std::string(regPath) + "\" /s");
  if (output.empty()) {
    std::cerr << "Warning: No Ubisoft Connect games found in registry"
              << std::endl;
    return games;
  }

  std::istringstream stream(output);
  std::string line, currentGameId;
  std::map<std::string, std::string> installDirs;

  while (std::getline(stream, line)) {
    line.erase(0, line.find_first_not_of(" \t\r\n"));
    line.erase(line.find_last_not_of(" \t\r\n") + 1);

    if (line.find("\\Installs\\") != std::string::npos) {
      size_t pos = line.rfind("\\");
      if (pos != std::string::npos) {
        currentGameId = line.substr(pos + 1);
      }
    } else if (!currentGameId.empty() &&
               line.find("InstallDir") != std::string::npos &&
               line.find("REG_SZ") != std::string::npos) {
      size_t pos = line.find("REG_SZ");
      if (pos != std::string::npos) {
        std::string path = line.substr(pos + 6);
        path.erase(0, path.find_first_not_of(" \t\r\n"));
        path.erase(path.find_last_not_of(" \t\r\n") + 1);
        installDirs[currentGameId] = path;
      }
    }
  }

  for (const auto& [gameId, dir] : installDirs) {
    if (fs::exists(dir)) {
      std::filesystem::path installPath(dir);

      if (installPath.filename().empty()) {
        installPath = installPath.parent_path();
      }

      std::string folderName = installPath.filename().string();
      Game game;
      game.appId = std::hash<std::string>{}("ubisoft:" + gameId);
      game.clientType = ClientType::UbisoftConnect;
      game.installPath = "";
      game.name = folderName;  // TODO: Replace this hack with actual solution
      game.executable = "uplay://launch/" + gameId + "/0";
      game.version = "unknown";
      game.sizeOnDisk = 0;
      game.bannerUrl = "";

      games.push_back(std::move(game));
    }
  }
#endif
  std::cout << "Found " << games.size() << " Ubisoft games." << std::endl;

  return games;
}

std::string UbisoftConnect::getName() const { return "Ubisoft Connect"; }

ClientType UbisoftConnect::getType() const {
  return ClientType::UbisoftConnect;
}
