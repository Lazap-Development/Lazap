#pragma once
#include <string>

class RichPresence {
 public:
  static void Initialize(const std::string& applicationId);
  static void UpdatePresence(const std::string& details,
                             const std::string& state);
  static void Shutdown();

 private:
  static bool initialized;
};
