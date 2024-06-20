#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class U16 : public Serializable {
  uint16_t value;

public:
  U16(uint16_t value = 0) : value{value} {}
  operator uint16_t() const { return value; }

  U16 operator+(const U16 &other) const { return U16(value + other.value); }
  U16 operator-(const U16 &other) const { return U16(value - other.value); }
  U16 operator*(const U16 &other) const { return U16(value * other.value); }
  U16 operator/(const U16 &other) const { return U16(value / other.value); }
  U16 operator%(const U16 &other) const { return U16(value % other.value); }

  bool operator==(const U16 &other) const { return value == other.value; }
  bool operator!=(const U16 &other) const { return value != other.value; }
  bool operator<(const U16 &other) const { return value < other.value; }
  bool operator<=(const U16 &other) const { return value <= other.value; }
  bool operator>(const U16 &other) const { return value > other.value; }
  bool operator>=(const U16 &other) const { return value >= other.value; }

  U16 operator&(const U16 &other) const { return U16(value & other.value); }
  U16 operator|(const U16 &other) const { return U16(value | other.value); }
  U16 operator^(const U16 &other) const { return U16(value ^ other.value); }
  U16 operator~() const { return U16(~value); }
  U16 operator<<(const U16 &other) const { return U16(value << other.value); }
  U16 operator>>(const U16 &other) const { return U16(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    uint16_t tempValue = value;
    memcpy(bytes.data(), &tempValue, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value = 0;
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    value = 0;
    memcpy(&value, bytes.data(), sizeof(value));
    return sizeof(value);
  }
};