#pragma once
#include "Serializable.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>

#include <stdexcept>

class I64 : public Serializable {
  int64_t value;

public:
  I64(int64_t value = 0) : value{value} {}
  operator int64_t() const { return value; }

  I64 operator+(const I64 &other) const { return I64(value + other.value); }
  I64 operator-(const I64 &other) const { return I64(value - other.value); }
  I64 operator*(const I64 &other) const { return I64(value * other.value); }
  I64 operator/(const I64 &other) const { return I64(value / other.value); }
  I64 operator%(const I64 &other) const { return I64(value % other.value); }

  bool operator==(const I64 &other) const { return value == other.value; }
  bool operator!=(const I64 &other) const { return value != other.value; }
  bool operator<(const I64 &other) const { return value < other.value; }
  bool operator<=(const I64 &other) const { return value <= other.value; }
  bool operator>(const I64 &other) const { return value > other.value; }
  bool operator>=(const I64 &other) const { return value >= other.value; }

  I64 operator&(const I64 &other) const { return I64(value & other.value); }
  I64 operator|(const I64 &other) const { return I64(value | other.value); }
  I64 operator^(const I64 &other) const { return I64(value ^ other.value); }
  I64 operator~() const { return I64(~value); }
  I64 operator<<(const I64 &other) const { return I64(value << other.value); }
  I64 operator>>(const I64 &other) const { return I64(value >> other.value); }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    uint64_t tempValue = 0;
    memcpy(&tempValue, &value, sizeof(int64_t));
    tempValue = hton64(tempValue);
    std::vector<unsigned char> bytes(sizeof(int64_t), 0);
    memcpy(bytes.data(), &tempValue, sizeof(int64_t));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value = 0;
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    memcpy(&value, bytes.data(), sizeof(int64_t));
    uint64_t *tempValue = (uint64_t *)&value;
    *tempValue = ntoh64(*tempValue);
    return sizeof(value);
  }
};