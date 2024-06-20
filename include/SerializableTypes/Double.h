#pragma once

#include "Serializable.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class Double : public Serializable {
  double value;

public:
  Double(double value = 0) : value{value} {}
  operator double() const { return value; }

  Double operator+(const Double &other) const {
    return Double(value + other.value);
  }
  Double operator-(const Double &other) const {
    return Double(value - other.value);
  }
  Double operator*(const Double &other) const {
    return Double(value * other.value);
  }
  Double operator/(const Double &other) const {
    return Double(value / other.value);
  }
  Double operator%(const Double &other) const {
    return Double(fmod(value, other.value));
  }

  bool operator==(const Double &other) const { return value == other.value; }
  bool operator!=(const Double &other) const { return value != other.value; }
  bool operator<(const Double &other) const { return value < other.value; }
  bool operator<=(const Double &other) const { return value <= other.value; }
  bool operator>(const Double &other) const { return value > other.value; }
  bool operator>=(const Double &other) const { return value >= other.value; }

  size_t getSize() override { return sizeof(value); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    uint64_t tempValue = 0;
    memcpy(&tempValue, &value, sizeof(double));
    tempValue = hton64(tempValue);
    bytes.insert(bytes.end(), (unsigned char *)&tempValue,
                 (unsigned char *)&tempValue + sizeof(double));
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    if (bytes.size() < sizeof(value)) {
      throw std::runtime_error("Too few bytes to deserialize");
    }
    value = 0;
    memcpy(&value, bytes.data(), sizeof(value));
    uint64_t *tempValue = (uint64_t *)&value;
    *tempValue = ntoh64(*tempValue);
    return sizeof(value);
  }
};