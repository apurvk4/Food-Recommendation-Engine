#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"

template <typename T>
using is_serializable_t =
    decltype(std::declval<T &>().serialize(std::declval<std::ostream &>()));

template <typename T1, typename T2,
          typename = std::enable_if_t<std::is_base_of_v<Serializable, T1> &&
                                      std::is_base_of_v<Serializable, T2>>>
class Pair : public Serializable {
public:
  T1 first;
  T2 second;
  Pair(T1 first, T2 second) : first(first), second(second) {}
  Pair() = default;
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> firstSerialized = first.serialize();
    std::vector<unsigned char> secondSerialized = second.serialize();
    serialized.insert(serialized.end(), firstSerialized.begin(),
                      firstSerialized.end());
    serialized.insert(serialized.end(), secondSerialized.begin(),
                      secondSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> firstData(data.begin(),
                                         data.begin() + first.getSize());
    first.deserialize(firstData);
    std::vector<unsigned char> secondData(data.begin() + first.getSize(),
                                          data.end());
    second.deserialize(secondData);
    return first.getSize() + second.getSize();
  }
  size_t getSize() override { return first.getSize() + second.getSize(); }
};