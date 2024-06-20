#pragma once

#include "Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <cstring> // For memcpy
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class SString : public Serializable {
  std::string value;

public:
  SString(std::string value) : value{value} {}
  SString() : value{""} { value.clear(); }

  // Conversion operator
  operator std::string() const { return value; }

  // Assignment operator
  SString &operator=(const std::string &other) {
    value = other;
    return *this;
  }

  // Setter
  void setValue(const std::string &value) { this->value = value; }

  // Getter
  std::string getValue() const { return value; }

  // Equality operators
  bool operator==(const SString &other) const { return value == other.value; }
  bool operator!=(const SString &other) const { return value != other.value; }

  // Stream extraction operator
  friend std::istream &operator>>(std::istream &is, SString &s) {
    std::getline(is, s.value);
    return is;
  }

  // Serialization
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    U64 size = value.size();
    auto sizeSer = size.serialize();
    bytes.insert(bytes.end(), sizeSer.begin(), sizeSer.end());
    bytes.insert(bytes.end(), value.begin(), value.end());
    return bytes;
  }

  // Deserialization
  size_t deserialize(const std::vector<unsigned char> &bytes) override {
    if (bytes.size() < sizeof(uint64_t)) {
      throw std::runtime_error("Not enough bytes to deserialize SString");
    }
    U64 size = 0;
    uint64_t bytesRead = size.deserialize(bytes);
    if (bytes.size() < sizeof(uint64_t) + size) {
      throw std::runtime_error("Not enough bytes to deserialize SString");
    }

    value = std::string(bytes.begin() + sizeof(uint64_t),
                        bytes.begin() + sizeof(uint64_t) + size);
    bytesRead += value.size();
    return bytesRead;
  }
  size_t getSize() override {
    std::cout << "size : " << value.size() << "\n";
    std::cout << "sizeof(uint64_t) : 8" << "\n";
    return 8 + value.size();
  }
};
