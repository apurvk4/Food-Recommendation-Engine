#pragma once

#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class I16 : public Serializable {
  int16_t value;

public:
  I16(int16_t value = 0) : value{value} {}
  operator int16_t() const { return value; }

  I16 operator+(const I16 &other) const { return I16(value + other.value); }
  I16 operator-(const I16 &other) const { return I16(value - other.value); }
  I16 operator*(const I16 &other) const { return I16(value * other.value); }
  I16 operator/(const I16 &other) const { return I16(value / other.value); }
  I16 operator%(const I16 &other) const { return I16(value % other.value); }

  bool operator==(const I16 &other) const { return value == other.value; }
  bool operator!=(const I16 &other) const { return value != other.value; }
  bool operator<(const I16 &other) const { return value < other.value; }
  bool operator<=(const I16 &other) const { return value <= other.value; }
  bool operator>(const I16 &other) const { return value > other.value; }
  bool operator>=(const I16 &other) const { return value >= other.value; }

  I16 operator&(const I16 &other) const { return I16(value & other.value); }
  I16 operator|(const I16 &other) const { return I16(value | other.value); }
  I16 operator^(const I16 &other) const { return I16(value ^ other.value); }
  I16 operator~() const { return I16(~value); }
  I16 operator<<(const I16 &other) const { return I16(value << other.value); }
  I16 operator>>(const I16 &other) const { return I16(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    uint16_t *tempValue = (uint16_t *)&value;
    memcpy(bytes.data(), tempValue, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value = 0;
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    memcpy(&value, bytes.data(), sizeof(value));
    uint16_t *tempValue = (uint16_t *)&value;
    return sizeof(value);
  }
};