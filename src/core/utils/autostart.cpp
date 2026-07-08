#ifdef _WIN32
#include <windows.h>

#include <shlobj.h>
#include <shlwapi.h>

#include <iostream>
#include <vector>

#endif

#ifdef __linux__
#include <limits.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#endif

#include <filesystem>
#include <string>

namespace Autostart {

#ifdef _WIN32

std::wstring getExecutablePath() {
  DWORD bufferSize = MAX_PATH;
  std::vector<wchar_t> buffer(bufferSize);

  DWORD result = GetModuleFileNameW(NULL, buffer.data(), bufferSize);

  while (result == buffer.size()) {
    bufferSize *= 2;
    buffer.resize(bufferSize);
    result = GetModuleFileNameW(NULL, buffer.data(), bufferSize);
  }

  if (result == 0) {
    return L"";
  }

  return std::wstring(buffer.data());
}

HRESULT createShortcut() {
  HRESULT hres;
  IShellLinkW* psl;
  std::wstring exePath = getExecutablePath();
  CoInitialize(NULL);

  hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                          IID_IShellLinkW, (LPVOID*)&psl);
  if (SUCCEEDED(hres)) {
    psl->SetPath(exePath.c_str());

    IPersistFile* ppf;
    hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
    if (SUCCEEDED(hres)) {
      PWSTR startupPath = NULL;
      hres = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &startupPath);

      if (SUCCEEDED(hres)) {
        std::wstring linkPath = std::wstring(startupPath) + L"\\Lazap.lnk";
        hres = ppf->Save(linkPath.c_str(), TRUE);
        CoTaskMemFree(startupPath);
      }
      ppf->Release();
    }
    psl->Release();
  }
  CoUninitialize();
  return hres;
}

#endif

#ifdef __linux__

std::string getExecutablePath() {
  char res[PATH_MAX];
  size_t count = readlink("/proc/self/exe", res, PATH_MAX);
  return std::string(res, count > 0 ? count : 0);
}

bool createShortcut() {
  const char* home = getenv("HOME");
  if (!home) return false;

  std::filesystem::path dir =
      std::filesystem::path(home) / ".config" / "autostart";

  if (!std::filesystem::exists(dir)) std::filesystem::create_directories(dir);

  std::filesystem::path file = dir / "lazap.desktop";
  std::string execPath = getExecutablePath();
  std::ofstream(file) << ("[Desktop Entry]\nName=Lazap\nExec=" + execPath +
                          "\nTerminal=false\nType=Application\nIcon="
                          "lazap\nCategories=Game;Utility;\n");
  return true;
}
#endif

bool deleteShortcut() {
#ifdef _WIN32
  PWSTR pszPath = NULL;
  std::wstring path;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &pszPath);

  if (SUCCEEDED(hr)) {
    path = (pszPath);

    CoTaskMemFree(pszPath);
  }
  try {
    return std::filesystem::remove(std::filesystem::path(path) / "Lazap.lnk");
  } catch (const std::filesystem::filesystem_error& e) {
    return false;
  }
#endif

#ifdef __linux__
  const char* home = getenv("HOME");
  if (!home) return false;

  return std::filesystem::remove(std::filesystem::path(home) / ".config" /
                                 "autostart" / "lazap.desktop");
#endif
}

bool shortcutExists() {
#ifdef _WIN32
  PWSTR pszPath = NULL;
  std::wstring path;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &pszPath);

  if (SUCCEEDED(hr)) {
    path = (pszPath);

    CoTaskMemFree(pszPath);
  }
  try {
    return std::filesystem::exists(std::filesystem::path(path) / "Lazap.lnk");
  } catch (const std::filesystem::filesystem_error& e) {
    return false;
  }
#endif

#ifdef __linux__
  const char* home = getenv("HOME");
  if (!home) return false;

  return std::filesystem::exists(std::filesystem::path(home) / ".config" /
                                 "autostart" / "lazap.desktop");
#endif
}
}  // namespace Autostart