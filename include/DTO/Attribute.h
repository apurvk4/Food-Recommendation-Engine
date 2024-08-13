#pragma once

#include "SerializableTypes/SerializableTypes.h"
#include <cstddef>
#include <stdexcept>

namespace DTO {

struct Attribute : public Serializable {
  U64 attributeId;
  SString attributeName;

  Attribute() = default;
  Attribute(U64 attributeId, SString attributeName)
      : attributeId(attributeId), attributeName(attributeName) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    auto IdSerialized = attributeId.serialize();
    auto NameSerialized = attributeName.serialize();
    bytes.insert(bytes.end(), IdSerialized.begin(), IdSerialized.end());
    bytes.insert(bytes.end(), NameSerialized.begin(), NameSerialized.end());
    return bytes;
  };
  size_t deserialize(const std::vector<unsigned char> &data) override {
    size_t bytesRead = attributeId.deserialize(data);
    bytesRead += attributeName.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  };
  std::size_t getSize() override {
    return attributeId.getSize() + attributeName.getSize();
  }
};

}; // namespace DTO