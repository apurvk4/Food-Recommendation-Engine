#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class U8 : public Serializable {
  uint8_t value;

public:
  U8(uint8_t value = 0) : value{value} {}
  operator uint8_t() const { return value; }

  U8 operator+(const U8 &other) const { return U8(value + other.value); }
  U8 operator-(const U8 &other) const { return U8(value - other.value); }
  U8 operator*(const U8 &other) const { return U8(value * other.value); }
  U8 operator/(const U8 &other) const { return U8(value / other.value); }
  U8 operator%(const U8 &other) const { return U8(value % other.value); }

  bool operator==(const U8 &other) const { return value == other.value; }
  bool operator!=(const U8 &other) const { return value != other.value; }
  bool operator<(const U8 &other) const { return value < other.value; }
  bool operator<=(const U8 &other) const { return value <= other.value; }
  bool operator>(const U8 &other) const { return value > other.value; }
  bool operator>=(const U8 &other) const { return value >= other.value; }

  U8 operator&(const U8 &other) const { return U8(value & other.value); }
  U8 operator|(const U8 &other) const { return U8(value | other.value); }
  U8 operator^(const U8 &other) const { return U8(value ^ other.value); }
  U8 operator~() const { return U8(~value); }
  U8 operator<<(const U8 &other) const { return U8(value << other.value); }
  U8 operator>>(const U8 &other) const { return U8(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    memcpy(bytes.data(), &value, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    if (bytes.size() < 1) {
      throw std::runtime_error("Invalid byte array size");
    }
    value = 0;
    memcpy(&value, bytes.data(), sizeof(value));
    return sizeof(value);
  }
};
