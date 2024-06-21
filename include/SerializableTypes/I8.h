#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class I8 : public Serializable {
  int8_t value;

public:
  I8(int8_t value = 0) : value{value} {}
  operator int8_t() const { return value; }

  I8 operator+(const I8 &other) const { return I8(value + other.value); }
  I8 operator-(const I8 &other) const { return I8(value - other.value); }
  I8 operator*(const I8 &other) const { return I8(value * other.value); }
  I8 operator/(const I8 &other) const { return I8(value / other.value); }
  I8 operator%(const I8 &other) const { return I8(value % other.value); }

  bool operator==(const I8 &other) const { return value == other.value; }
  bool operator!=(const I8 &other) const { return value != other.value; }
  bool operator<(const I8 &other) const { return value < other.value; }
  bool operator<=(const I8 &other) const { return value <= other.value; }
  bool operator>(const I8 &other) const { return value > other.value; }
  bool operator>=(const I8 &other) const { return value >= other.value; }

  I8 operator&(const I8 &other) const { return I8(value & other.value); }
  I8 operator|(const I8 &other) const { return I8(value | other.value); }
  I8 operator^(const I8 &other) const { return I8(value ^ other.value); }
  I8 operator~() const { return I8(~value); }
  I8 operator<<(const I8 &other) const { return I8(value << other.value); }
  I8 operator>>(const I8 &other) const { return I8(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    memcpy(bytes.data(), &value, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value = 0;
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    memcpy(&value, bytes.data(), sizeof(value));
    return sizeof(value);
  }
};