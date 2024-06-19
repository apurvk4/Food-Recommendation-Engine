#pragma once
#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>

#include <stdexcept>

class I32 : public Serializable {
  int32_t value;

public:
  I32(int32_t value = 0) : value{value} {}
  operator int32_t() const { return value; }

  I32 operator+(const I32 &other) const { return I32(value + other.value); }
  I32 operator-(const I32 &other) const { return I32(value - other.value); }
  I32 operator*(const I32 &other) const { return I32(value * other.value); }
  I32 operator/(const I32 &other) const { return I32(value / other.value); }
  I32 operator%(const I32 &other) const { return I32(value % other.value); }

  bool operator==(const I32 &other) const { return value == other.value; }
  bool operator!=(const I32 &other) const { return value != other.value; }
  bool operator<(const I32 &other) const { return value < other.value; }
  bool operator<=(const I32 &other) const { return value <= other.value; }
  bool operator>(const I32 &other) const { return value > other.value; }
  bool operator>=(const I32 &other) const { return value >= other.value; }

  I32 operator&(const I32 &other) const { return I32(value & other.value); }
  I32 operator|(const I32 &other) const { return I32(value | other.value); }
  I32 operator^(const I32 &other) const { return I32(value ^ other.value); }
  I32 operator~() const { return I32(~value); }
  I32 operator<<(const I32 &other) const { return I32(value << other.value); }
  I32 operator>>(const I32 &other) const { return I32(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes(sizeof(value), 0);
    uint32_t tempValue = (uint32_t)value;
    tempValue = htons(tempValue);
    memcpy(bytes.data(), &tempValue, sizeof(value));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value = 0;
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    memcpy(&value, bytes.data(), sizeof(value));
    uint32_t *tempValue = (uint32_t *)&value;
    *tempValue = htons(*tempValue);
    return sizeof(value);
  }
};