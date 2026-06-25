#include "utils/update_manager.h"

#include <cpr/cpr.h>

#include <glaze/glaze.hpp>
#include <iostream>

#include "string.h"
#include "version.h"

struct UpdateRes {
  std::string tag_name;
  std::string html_url;
};

template <>
struct glz::meta<UpdateRes> {
  static constexpr auto value = object("tag_name", &UpdateRes::tag_name,
                                       "html_url", &UpdateRes::html_url);
};

struct Version {
  int major;
  int minor;
  int patch;
};

Version parseVersion(std::string version) {
  Version result{};

  if (!version.empty() && version[0] == 'v') version.erase(0, 1);

  sscanf(version.data(), "%d.%d.%d", &result.major, &result.minor,
         &result.patch);

  return result;
}

bool isNewer(const Version& remote, const Version& local) {
  if (remote.major != local.major) return remote.major > local.major;

  if (remote.minor != local.minor) return remote.minor > local.minor;

  return remote.patch > local.patch;
}

namespace Updater {
void checkForUpdates() {
  cpr::Response res = cpr::Get(cpr::Url{
      "https://api.github.com/repos/Lazap-Development/Lazap/releases/latest"});

  if (res.status_code != 200) {
    std::cerr << "HTTP Error: " << res.status_code << std::endl;
    return;
  }

  UpdateRes obj;

  if (auto ec =
          glz::read<glz::opts{.error_on_unknown_keys = false}>(obj, res.text);
      ec) {
    std::cerr << glz::format_error(ec, res.text) << std::endl;
    return;
  }

  glz::read_json(obj, res.text);

  auto remoteVersion = parseVersion(obj.tag_name);
  auto localVersion = parseVersion(APP_VERSION);

  if (isNewer(remoteVersion, localVersion)) {
    std::cout << "Remote Version: " << obj.tag_name << std::endl;
    std::cout << "Local Version: " << APP_VERSION << std::endl;

    hasUpdate = true;
    url = obj.html_url;
  }
}
}  // namespace Updater