#include <string>

#include "clients/client.h"

class BannerManager {
 public:
  std::string getBanner(ClientType client, const std::string& displayName,
                        const std::string& gameId);

 private:
  std::string fetchBanner(const std::string& url,
                          const std::string& displayName);
  std::string rawgFetchBanner(const std::string& displayName);
  std::string replaceAll(const std::string& str, const std::string& from,
                         const std::string& to);
  std::string parseRawgResponse(const std::string& json);
};
