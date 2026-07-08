#pragma once
#include <string>

namespace Updater {
void checkForUpdates();
inline bool hasUpdate;
inline std::string url;
}  // namespace Updater