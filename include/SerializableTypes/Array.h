#pragma once

#include "Serializable.h"
#include "U64.h"
#include <cstddef>
#include <iostream>

template <typename T,
          typename = std::enable_if_t<std::is_base_of<Serializable, T>::value>>
class Array : public Serializable {
  std::vector<T> value;

public:
  Array(std::vector<T> value) : value{value} {}
  Array() = default;
  operator std::vector<T>() const { return value; }
  size_t NumberOfItems() { return value.size(); }
  T &operator[](int index) { return value[index]; }

  size_t getSize() override {
    U64 vectorSize = value.size();
    size_t size = vectorSize.getSize();
    for (auto &e : value) {
      size += e.getSize();
    }
    return size;
  }
  void push_back(T element) { value.push_back(element); }

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    U64 size = value.size();
    std::vector<unsigned char> sizeBytes = size.serialize();
    bytes.insert(bytes.end(), sizeBytes.begin(), sizeBytes.end());
    for (T &element : value) {
      std::vector<unsigned char> elementBytes = element.serialize();
      bytes.insert(bytes.end(), elementBytes.begin(), elementBytes.end());
    }
    std::cout << "Serialized array size : " << bytes.size() << std::endl;
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    value.clear();
    U64 size = (U64)0;
    std::cout << "received payload size : " << bytes.size() << std::endl;
    uint64_t bytesRead = size.deserialize(bytes);
    for (uint64_t i = 0; i < size; i++) {
      T element;
      bytesRead += element.deserialize(
          std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
      value.push_back(element);
    }
    return bytesRead;
  }
};