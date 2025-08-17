#include <clients/steam/appinfo.h>

#include <cstring>
#include <fstream>
#include <stdexcept>

BinaryReader::BinaryReader(const uint8_t* data, size_t size)
    : data(data), size(size), pos(0) {}

bool BinaryReader::canRead(size_t bytes) const { return pos + bytes <= size; }

template <typename T>
T BinaryReader::read() {
  if (!canRead(sizeof(T))) {
    throw std::runtime_error("Read beyond bounds");
  }
  T value;
  std::memcpy(&value, data + pos, sizeof(T));
  pos += sizeof(T);
  return value;
}

std::string BinaryReader::readString() {
  std::string result;
  while (canRead(1)) {
    char c = read<char>();
    if (c == '\0') break;
    result += c;
  }
  return result;
}

const uint8_t* BinaryReader::getCurrentPtr() const { return data + pos; }

size_t BinaryReader::getPos() const { return pos; }

void BinaryReader::setPos(size_t newPos) {
  if (newPos <= size) pos = newPos;
}

size_t BinaryReader::remaining() const { return size - pos; }

template uint8_t BinaryReader::read<uint8_t>();
template uint32_t BinaryReader::read<uint32_t>();
template uint64_t BinaryReader::read<uint64_t>();
template char BinaryReader::read<char>();

void AppInfoParser::parseStringTable(BinaryReader& reader, size_t offset) {
  if (offset >= data.size()) return;

  reader.setPos(offset);
  if (!reader.canRead(4)) return;

  uint32_t numStrings = reader.read<uint32_t>();
  if (numStrings > 1000000) return;

  strings.clear();
  strings.reserve(numStrings);

  const char* stringData =
      reinterpret_cast<const char*>(reader.getCurrentPtr());
  const char* end = reinterpret_cast<const char*>(data.data() + data.size());

  strings.push_back(stringData);

  size_t strCount = 1;
  const char* current = stringData;

  while (current < end && strCount < numStrings) {
    if (*current == '\0') {
      current++;
      if (current < end) {
        strings.push_back(current);
        strCount++;
      }
    } else {
      current++;
    }
  }
}

void AppInfoParser::parseKeyValue(BinaryReader& reader,
                                  const std::string& parentKey = "") {
  while (reader.canRead(1)) {
    TokenOp op = static_cast<TokenOp>(reader.read<uint8_t>());
    if (op == SectionEnd) {
      return;
    }
    std::string keyName;
    if (vdf_version >= 0x29) {
      if (!reader.canRead(4)) break;
      uint32_t strIndex = reader.read<uint32_t>();
      if (strIndex < strings.size()) {
        keyName = strings[strIndex];
      } else {
        continue;
      }
    } else {
      keyName = reader.readString();
      if (keyName.empty()) continue;
    }
    switch (op) {
      case SectionBegin: {
        if (parentKey == "launch") {
          // Check if keyName is a number (0, 1, 2, etc.)
          bool isNumber =
              !keyName.empty() &&
              std::all_of(keyName.begin(), keyName.end(), ::isdigit);
          if (isNumber) {
            executables.push_back({});
          }
        }
        parseKeyValue(reader, keyName);
        break;
      }
      case String: {
        std::string value = reader.readString();
        if ((keyName == "executable" || keyName == "oslist" ||
             keyName == "BetaKey") &&
            !executables.empty()) {
          executables.back()[keyName] = value;
        }
        break;
      }
      case Int32:
        if (!reader.canRead(4)) return;
        reader.read<uint32_t>();
        break;
      case Int64:
        if (!reader.canRead(8)) return;
        reader.read<uint64_t>();
        break;
      default:
        return;
    }
  }
}

bool AppInfoParser::loadFile(const std::string& filepath) {
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    return false;
  }

  file.seekg(0, std::ios::end);
  size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  data.resize(fileSize);
  file.read(reinterpret_cast<char*>(data.data()), fileSize);
  file.close();

  return data.size() >= 8;
}

std::vector<std::map<std::string, std::string>> AppInfoParser::getLaunchConfig(
    uint32_t targetAppId) {
  executables.clear();

  BinaryReader reader(data.data(), data.size());

  uint32_t version = reader.read<uint32_t>();
  /* uint32_t universe = */ reader.read<uint32_t>();
  vdf_version = version & 0xFF;

  if (vdf_version >= 0x29) {
    if (reader.canRead(8)) {
      uint64_t stringTableOffset = reader.read<uint64_t>();
      if (stringTableOffset < data.size()) {
        size_t savedPos = reader.getPos();
        parseStringTable(reader, stringTableOffset);
        reader.setPos(savedPos);
      }
    }
  }

  while (reader.remaining() > 0) {
    if (!reader.canRead(4)) break;
    uint32_t appid = reader.read<uint32_t>();
    if (appid == 0) break;

    if (!reader.canRead(4)) break;
    uint32_t appSize = reader.read<uint32_t>();

    if (appid != targetAppId) {
      reader.setPos(reader.getPos() + appSize);
      continue;
    }

    if (reader.canRead(4)) reader.read<uint32_t>();
    if (reader.canRead(4)) reader.read<uint32_t>();
    if (reader.canRead(8)) reader.read<uint64_t>();
    if (reader.canRead(20)) reader.setPos(reader.getPos() + 20);
    if (reader.canRead(4)) reader.read<uint32_t>();
    if (vdf_version >= 0x28 && reader.canRead(20)) {
      reader.setPos(reader.getPos() + 20);
    }

    if (appSize > 0 && reader.canRead(appSize)) {
      try {
        parseKeyValue(reader);
        return executables;
      } catch (...) {
        return {};
      }
    }
    break;
  }
  return {};
}
