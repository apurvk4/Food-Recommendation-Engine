#pragma once

#include "FoodItem.h"
#include "MenuItem.h"
#include "MenuType.h"
#include <cstdint>
#include <string>
#include <vector>

namespace DTO {

struct Menu : public Serializable {
  U64 menuId;
  SString menuName;
  U64 menuTypeId;
  Menu(U64 menuId, SString menuName, U64 menuTypeId)
      : menuId(menuId), menuName(menuName), menuTypeId(menuTypeId) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> menuNameSerialized = menuName.serialize();
    std::vector<unsigned char> menuTypeIdSerialized = menuTypeId.serialize();
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), menuNameSerialized.begin(),
                      menuNameSerialized.end());
    serialized.insert(serialized.end(), menuTypeIdSerialized.begin(),
                      menuTypeIdSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> menuIdData(data.begin(), data.begin() + 8);
    menuId.deserialize(menuIdData);
    std::vector<unsigned char> menuNameData(data.begin() + 8,
                                            data.begin() + 40);
    menuName.deserialize(menuNameData);
    std::vector<unsigned char> menuTypeIdData(data.begin() + 40,
                                              data.begin() + 48);
    menuTypeId.deserialize(menuTypeIdData);
    return 48;
  }
  size_t getSize() override { return 48; }
};

}; // namespace DTO