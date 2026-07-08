#ifdef _WIN32
#include <shlwapi.h>
#endif

#include <string>

namespace Autostart {
#ifdef _WIN32
std::wstring getExecutablePath();
HRESULT createShortcut();
#endif

#ifdef __linux__
std::string getExecutablePath();
bool createShortcut();
#endif

bool deleteShortcut();
bool shortcutExists();
}  // namespace Autostart