#pragma once

#include "Serializable.h"
#include "U64.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

class SString : public Serializable {
  std::string value;

public:
  SString(std::string value = "") : value{value} {}
  operator std::string() const { return value; }

  SString operator+(const SString &other) const {
    return SString(value + other.value);
  }

  bool operator==(const SString &other) const { return value == other.value; }
  bool operator!=(const SString &other) const { return value != other.value; }
  bool operator<(const SString &other) const { return value < other.value; }
  bool operator<=(const SString &other) const { return value <= other.value; }
  bool operator>(const SString &other) const { return value > other.value; }
  bool operator>=(const SString &other) const { return value >= other.value; }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    U64 size(value.size());
    std::vector<unsigned char> sizeBytes = size.serialize();
    bytes.insert(bytes.end(), sizeBytes.begin(), sizeBytes.end());
    bytes.insert(bytes.end(), value.begin(), value.end());
    return bytes;
  }

  size_t getSize() override {
    return value.size() * sizeof(char) + sizeof(uint64_t);
  }

  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value.clear();
    if (bytes.size() < sizeof(uint64_t)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }

    // Deserialize the size
    U64 size;
    uint64_t bytesRead =
        size.deserialize({bytes.begin(), bytes.begin() + sizeof(uint64_t)});

    if (bytes.size() < size + (U64)bytesRead) {
      throw std::runtime_error("Too few bytes to deserialize");
    }

    // Deserialize the string content
    value = std::string(bytes.begin() + bytesRead,
                        bytes.begin() + bytesRead + size);

    return bytesRead + size;
  }
};
