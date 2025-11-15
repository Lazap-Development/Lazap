#ifndef FNV1A_HPP
#define FNV1A_HPP

#include <cstddef>
#include <cstdint>

namespace fnv1a {

// FNV-1a 64-bit constants
constexpr uint64_t FNV_offset_basis_64 = 14695981039346656037ULL;
constexpr uint64_t FNV_prime_64 = 1099511628211ULL;

uint64_t hash(const char* data, std::size_t length);

}  // namespace fnv1a

#endif
