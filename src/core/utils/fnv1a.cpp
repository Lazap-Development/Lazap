// fnv1a.cpp
#include "utils/fnv1a.h"

namespace fnv1a {

uint64_t hash(const char* data, std::size_t length) {
  uint64_t hash = FNV_offset_basis_64;
  for (std::size_t i = 0; i < length; ++i) {
    hash ^= static_cast<uint8_t>(data[i]);
    hash *= FNV_prime_64;
  }
  return hash;
}

}  // namespace fnv1a
