#pragma once
#include <cstdint>
#include <vector>

class Serializable {
public:
  virtual std::vector<unsigned char> serialize() = 0;
  virtual uint64_t deserialize(const std::vector<unsigned char> &) = 0;
  virtual std::size_t getSize() = 0;
  virtual ~Serializable() = default;
};