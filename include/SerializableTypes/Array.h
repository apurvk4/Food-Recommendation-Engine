#pragma once

#include "Serializable.h"
#include "U64.h"

template <typename T,
          typename = std::enable_if_t<std::is_base_of<Serializable, T>::value>>
class Array : public Serializable {
  std::vector<T> value;

public:
  Array(std::vector<T> value = {}) : value{value} {}
  operator std::vector<T>() const { return value; }

  Array operator+(const Array &other) const {
    return Array(value + other.value);
  }

  bool operator==(const Array &other) const { return value == other.value; }
  bool operator!=(const Array &other) const { return value != other.value; }
  bool operator<(const Array &other) const { return value < other.value; }
  bool operator<=(const Array &other) const { return value <= other.value; }
  bool operator>(const Array &other) const { return value > other.value; }
  bool operator>=(const Array &other) const { return value >= other.value; }

  T &operator[](int index) { return value[index]; }

  Array operator<<(const Array &other) const {
    return Array(value << other.value);
  }
  Array operator>>(const Array &other) const {
    return Array(value >> other.value);
  }

  size_t getSize() override {
    size_t size = 0;
    if (value.size() > 0) {
      size = value[0].getSize() * value.size();
    }
    return size;
  }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    U64 size = value.size();
    std::vector<unsigned char> sizeBytes = size.serialize();
    bytes.insert(bytes.end(), sizeBytes.begin(), sizeBytes.end());
    for (T &element : value) {
      std::vector<unsigned char> elementBytes = element.serialize();
      bytes.insert(bytes.end(), elementBytes.begin(), elementBytes.end());
    }
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    U64 size = (U64)0;
    uint64_t bytesRead = size.deserialize(bytes);
    value.clear();
    for (uint64_t i = 0; i < size; i++) {
      T element;
      bytesRead += element.deserialize(
          std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
      value.push_back(element);
    }
    return bytesRead;
  }
};