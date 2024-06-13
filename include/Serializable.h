#pragma once
#include <cstdint>
#include <vector>

inline uint64_t hton64(uint64_t host64) {
  uint64_t result = ((host64 & 0x00000000000000FFULL) << 56) |
                    ((host64 & 0x000000000000FF00ULL) << 40) |
                    ((host64 & 0x0000000000FF0000ULL) << 24) |
                    ((host64 & 0x00000000FF000000ULL) << 8) |
                    ((host64 & 0x000000FF00000000ULL) >> 8) |
                    ((host64 & 0x0000FF0000000000ULL) >> 24) |
                    ((host64 & 0x00FF000000000000ULL) >> 40) |
                    ((host64 & 0xFF00000000000000ULL) >> 56);
  return result;
}

inline uint64_t ntoh64(uint64_t net64) {
  return hton64(net64); // Since hton64 and ntoh64 are symmetric
}

class Serializable {
public:
  virtual std::vector<unsigned char> serialize() = 0;
  virtual uint64_t deserialize(const std::vector<unsigned char> &) = 0;
  virtual std::size_t getSize() = 0;
  virtual ~Serializable() = default;
};