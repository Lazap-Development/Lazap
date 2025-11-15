#include "storage/storage_config.h"

#include <cstdlib>
#include <string>

namespace StorageConfig {
std::string getConfigPath() {
#ifdef _WIN32
  const char* appdata = std::getenv("APPDATA");
  if (appdata) {
    return std::string(appdata);
  }
  return "C:\\ProgramData";
#else
  const char* xdg_config = std::getenv("XDG_CONFIG_HOME");
  if (xdg_config) {
    return std::string(xdg_config);
  }
  const char* home = std::getenv("HOME");
  if (home) {
    return std::string(home) + "/.config";
  }
  return "/tmp";
#endif
}

const std::string CONFIG_DIR = getConfigPath();
const std::string APP_NAME = "lazap";
const std::string CONFIG_NAME = "config.toml";
const std::string STORAGE_DIR = CONFIG_DIR +
#ifdef _WIN32
                                "\\" + APP_NAME;
#else
                                "/" + APP_NAME;
#endif
}  // namespace StorageConfig
