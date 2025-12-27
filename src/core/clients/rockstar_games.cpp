#include <clients/rockstar_games.h>

#include <filesystem>
#include <iostream>
#include <regex>

#include "clients/client.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <glaze/glaze.hpp>

namespace fs = std::filesystem;

#if defined(_WIN32)
static std::string queryRegistry(const std::string& command) {
  STARTUPINFOA si{};
  si.cb = sizeof(si);
  PROCESS_INFORMATION pi{};
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;

  HANDLE readPipe, writePipe;
  SECURITY_ATTRIBUTES sa{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};

  if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) return "";

  si.hStdOutput = writePipe;
  si.hStdError = writePipe;

  std::string cmdLine = "cmd.exe /C " + command;
  if (!CreateProcessA(nullptr, &cmdLine[0], nullptr, nullptr, TRUE,
                      CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
    CloseHandle(readPipe);
    CloseHandle(writePipe);
    return "";
  }

  CloseHandle(writePipe);
  std::string output;
  char buffer[4096];
  DWORD bytesRead = 0;

  while (ReadFile(readPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) &&
         bytesRead > 0) {
    buffer[bytesRead] = '\0';
    output += buffer;
  }

  CloseHandle(readPipe);
  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return output;
}
#endif
static const char* ROCKSTAR_JSON = R"JSON(
{
  "titles": []
}
)JSON";

struct RockstarTitle {
  std::string titleId;
  int rosTitleId{};
  std::string friendlyName;
  std::string installFolderRegKey;
  std::string installFolderRegValueName;
  std::string executable;
  std::string folder;
  std::string bannerId;
  std::optional<std::vector<std::string>> aliases;
};

struct RockstarTitles {
  std::vector<RockstarTitle> titles;
};

template <>
struct glz::meta<RockstarTitle> {
  using T = RockstarTitle;
  static constexpr auto value = glz::object(
      "titleId", &T::titleId, "rosTitleId", &T::rosTitleId, "friendlyName",
      &T::friendlyName, "installFolderRegKey", &T::installFolderRegKey,
      "installFolderRegValueName", &T::installFolderRegValueName, "executable",
      &T::executable, "folder", &T::folder, "bannerId", &T::bannerId, "aliases",
      &T::aliases);
};

template <>
struct glz::meta<RockstarTitles> {
  using T = RockstarTitles;
  static constexpr auto value = glz::object("titles", &T::titles);
};

static inline std::string trim(const std::string& s) {
  auto start = s.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  auto end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

std::string RockstarGames::getLocation() {
#if defined(_WIN32)
#ifdef _WIN64
  const char* regPath =
      R"(HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Rockstar Games)";
#else
  const char* regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Rockstar Games";
#endif

  std::string output =
      queryRegistry("Reg query \"" + std::string(regPath) + "\" /s");
  if (output.empty()) return "";

  std::regex re(R"(InstallFolder\s+REG_SZ\s+([^\r\n]+))");
  std::smatch m;
  if (std::regex_search(output, m, re)) {
    return trim(m[1].str());
  }
#endif
  return "";
}

std::vector<Game> RockstarGames::getInstalledGames() {
  std::vector<Game> games;
#if defined(_WIN32)
#ifdef _WIN64
  const char* baseKey =
      R"(HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Rockstar Games)";
#else
  const char* baseKey = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Rockstar Games";
#endif

  std::string regOutput =
      queryRegistry("Reg query \"" + std::string(baseKey) + "\" /s");
  if (regOutput.empty()) {
    std::cerr << "Warning: No Rockstar Games registry entries found.\n";
    std::cout << "Found 0 Rockstar Games.\n";
    return games;
  }

  RockstarTitles titles;
  glz::read_json(titles, ROCKSTAR_JSON);

  std::istringstream input(regOutput);
  std::string line, currentKey;
  std::map<std::string, std::string> installs;

  while (std::getline(input, line)) {
    line = trim(line);
    if (line.empty()) continue;

    // detect section
    if (line.find("Rockstar Games\\") != std::string::npos) {
      currentKey = line.substr(line.find_last_of('\\') + 1);
    } else if (!currentKey.empty() &&
               line.find("InstallFolder") != std::string::npos &&
               line.find("REG_SZ") != std::string::npos) {
      size_t pos = line.find("REG_SZ");
      if (pos != std::string::npos) {
        std::string path = trim(line.substr(pos + 6));
        if (!path.empty()) installs[currentKey] = path;
      }
    }
  }

  for (const auto& [key, path] : installs) {
    if (!fs::exists(path)) continue;

    for (const auto& title : titles.titles) {
      if (title.friendlyName == key || title.titleId == key ||
          (title.aliases &&
           std::ranges::find(title.aliases->begin(), title.aliases->end(),
                             key) != title.aliases->end())) {
        Game g;
        g.appId = std::hash<std::string>{}("rockstar:" + title.titleId);
        g.clientType = ClientType::RockstarGames;
        g.installPath = path;
        g.name = title.friendlyName.empty() ? key : title.friendlyName;
        g.executable = title.executable;
        g.version = "unknown";
        g.sizeOnDisk = 0;
        g.bannerUrl = title.bannerId;

        games.push_back(std::move(g));
        break;
      }
    }
  }
#endif

  std::cout << "Found " << games.size() << " Rockstar Games.\n";
  return games;
}

ClientType RockstarGames::getType() const { return ClientType::RockstarGames; }
