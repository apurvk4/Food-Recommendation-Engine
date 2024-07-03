#pragma once

#include "FoodItem.h"
#include "SerializableTypes/U32.h"
#include <cstdint>

namespace DTO {

struct MenuItem : public Serializable {
  U64 menuItemId;
  U64 foodItemId;
  U64 menuId;
  U32 quantity;
  MenuItem() : menuItemId(0), foodItemId(0), menuId(0), quantity(0) {}
  MenuItem(U64 menuItemId, U64 foodItemId, U64 menuId, U32 quantity)
      : menuItemId(menuItemId), foodItemId(foodItemId), menuId(menuId),
        quantity(quantity) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> menuItemIdSerialized = menuItemId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> quantitySerialized = quantity.serialize();
    serialized.insert(serialized.end(), menuItemIdSerialized.begin(),
                      menuItemIdSerialized.end());
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), quantitySerialized.begin(),
                      quantitySerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    size_t bytesRead = 0;
    bytesRead += menuItemId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += foodItemId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += menuId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += quantity.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() override {
    return menuItemId.getSize() + foodItemId.getSize() + menuId.getSize() +
           quantity.getSize();
  }
};

}; // namespace DTO