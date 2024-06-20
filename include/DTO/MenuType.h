#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

class MenuType : public Serializable {
public:
  U64 menuTypeId;
  SString menuType;
  MenuType(U64 menuTypeId, SString menuType)
      : menuTypeId(menuTypeId), menuType(menuType) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> menuTypeIdSerialized = menuTypeId.serialize();
    std::vector<unsigned char> menuTypeSerialized = menuType.serialize();
    serialized.insert(serialized.end(), menuTypeIdSerialized.begin(),
                      menuTypeIdSerialized.end());
    serialized.insert(serialized.end(), menuTypeSerialized.begin(),
                      menuTypeSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> menuTypeIdData(data.begin(), data.begin() + 8);
    menuTypeId.deserialize(menuTypeIdData);
    std::vector<unsigned char> menuTypeData(data.begin() + 8,
                                            data.begin() + 40);
    menuType.deserialize(menuTypeData);
    return 40;
  }
  size_t getSize() override { return 40; }
};

}; // namespace DTO