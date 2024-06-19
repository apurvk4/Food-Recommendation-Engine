#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class U64 : public Serializable {
  uint64_t value;

public:
  U64(uint64_t value = 0) : value{value} {}
  operator uint64_t() const { return value; }

  U64 operator+(const U64 &other) const { return U64(value + other.value); }
  U64 operator-(const U64 &other) const { return U64(value - other.value); }
  U64 operator*(const U64 &other) const { return U64(value * other.value); }
  U64 operator/(const U64 &other) const { return U64(value / other.value); }
  U64 operator%(const U64 &other) const { return U64(value % other.value); }

  bool operator==(const U64 &other) const { return value == other.value; }
  bool operator!=(const U64 &other) const { return value != other.value; }
  bool operator<(const U64 &other) const { return value < other.value; }
  bool operator<=(const U64 &other) const { return value <= other.value; }
  bool operator>(const U64 &other) const { return value > other.value; }
  bool operator>=(const U64 &other) const { return value >= other.value; }

  U64 operator&(const U64 &other) const { return U64(value & other.value); }
  U64 operator|(const U64 &other) const { return U64(value | other.value); }
  U64 operator^(const U64 &other) const { return U64(value ^ other.value); }
  U64 operator~() const { return U64(~value); }
  U64 operator<<(const U64 &other) const { return U64(value << other.value); }
  U64 operator>>(const U64 &other) const { return U64(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    uint64_t tempValue = hton64(value);
    memcpy(bytes.data(), &tempValue, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    value = 0;
    memcpy(&value, bytes.data(), sizeof(value));
    value = ntoh64(value);
    return sizeof(value);
  }
};