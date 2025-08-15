#include <utils/launch_manager.h>

#include <cstdlib>
#include <filesystem>
#include <thread>

bool LaunchManager::launch(const ClientType &client,
                           const std::string &installPath,
                           const std::string &executable) {
  namespace fs = std::filesystem;
  std::string fullPath = (fs::path(installPath) / executable).string();

#ifdef _WIN32
  std::thread([fullPath] {
    std::string cmd = "start \"\" \"" + fullPath + "\"";
    std::system(cmd.c_str());
  }).detach();
  return true;

#elif defined(__linux__) || defined(__APPLE__)
  std::thread([fullPath] {
    if (fullPath.size() >= 4 &&
        fullPath.compare(fullPath.size() - 4, 4, ".exe") == 0) {
      std::string wineCmd = "wine \"" + fullPath + "\"";
      std::system(wineCmd.c_str());
    } else {
      std::system(("\"" + fullPath + "\"").c_str());
    }
  }).detach();
  return true;

#else
  return false;
#endif
}
