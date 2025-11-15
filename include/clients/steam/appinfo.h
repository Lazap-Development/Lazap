#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class BinaryReader {
 private:
  const uint8_t* data;
  size_t size;
  size_t pos;

 public:
  BinaryReader(const uint8_t* data, size_t size);

  bool canRead(size_t bytes) const;

  template <typename T>
  T read();

  std::string readString();

  const uint8_t* getCurrentPtr() const;
  size_t getPos() const;
  void setPos(size_t newPos);
  size_t remaining() const;
};

enum TokenOp {
  SectionBegin = 0x0,
  String = 0x1,
  Int32 = 0x2,
  Int64 = 0x7,
  SectionEnd = 0x8
};

class AppInfoParser {
 private:
  std::vector<uint8_t> data;
  uint8_t vdf_version = 0;
  std::vector<const char*> strings;
  std::vector<std::map<std::string, std::string>> executables;

  void parseStringTable(BinaryReader& reader, size_t offset);
  void parseKeyValue(BinaryReader& reader, const std::string& parentKey);

 public:
  bool loadFile(const std::string& filepath);
  std::vector<std::map<std::string, std::string>> getLaunchConfig(
      uint32_t targetAppId);
};
