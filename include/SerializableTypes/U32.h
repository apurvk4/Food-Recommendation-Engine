#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class U32 : public Serializable {
  uint32_t value;

public:
  U32(uint32_t value = 0) : value{value} {}
  operator uint32_t() const { return value; }

  U32 operator+(const U32 &other) const { return U32(value + other.value); }
  U32 operator-(const U32 &other) const { return U32(value - other.value); }
  U32 operator*(const U32 &other) const { return U32(value * other.value); }
  U32 operator/(const U32 &other) const { return U32(value / other.value); }
  U32 operator%(const U32 &other) const { return U32(value % other.value); }

  bool operator==(const U32 &other) const { return value == other.value; }
  bool operator!=(const U32 &other) const { return value != other.value; }
  bool operator<(const U32 &other) const { return value < other.value; }
  bool operator<=(const U32 &other) const { return value <= other.value; }
  bool operator>(const U32 &other) const { return value > other.value; }
  bool operator>=(const U32 &other) const { return value >= other.value; }

  U32 operator&(const U32 &other) const { return U32(value & other.value); }
  U32 operator|(const U32 &other) const { return U32(value | other.value); }
  U32 operator^(const U32 &other) const { return U32(value ^ other.value); }
  U32 operator~() const { return U32(~value); }
  U32 operator<<(const U32 &other) const { return U32(value << other.value); }
  U32 operator>>(const U32 &other) const { return U32(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    uint32_t tempValue = value;
    memcpy(bytes.data(), &tempValue, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    value = 0;
    memcpy(&value, bytes.data(), sizeof(value));
    return sizeof(value);
  }
};
