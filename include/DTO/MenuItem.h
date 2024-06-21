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
    std::vector<unsigned char> menuItemIdData(data.begin(), data.begin() + 8);
    menuItemId.deserialize(menuItemIdData);
    std::vector<unsigned char> foodItemIdData(data.begin() + 8,
                                              data.begin() + 16);
    foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> menuIdData(data.begin() + 16, data.begin() + 24);
    menuId.deserialize(menuIdData);
    std::vector<unsigned char> quantityData(data.begin() + 24,
                                            data.begin() + 28);
    quantity.deserialize(quantityData);
    return 28;
  }
  size_t getSize() override { return 28; }
};

}; // namespace DTO