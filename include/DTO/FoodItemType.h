#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct FoodItemType : public Serializable {
  U64 foodItemTypeId;
  SString foodItemType;
  FoodItemType(U64 foodItemTypeId, SString foodItemType)
      : foodItemTypeId(foodItemTypeId), foodItemType(foodItemType) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> foodItemTypeIdSerialized =
        foodItemTypeId.serialize();
    std::vector<unsigned char> foodItemTypeSerialized =
        foodItemType.serialize();
    serialized.insert(serialized.end(), foodItemTypeIdSerialized.begin(),
                      foodItemTypeIdSerialized.end());
    serialized.insert(serialized.end(), foodItemTypeSerialized.begin(),
                      foodItemTypeSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> foodItemTypeIdData(data.begin(),
                                                  data.begin() + 8);
    foodItemTypeId.deserialize(foodItemTypeIdData);
    std::vector<unsigned char> foodItemTypeData(data.begin() + 8,
                                                data.begin() + 40);
    foodItemType.deserialize(foodItemTypeData);
    return 40;
  }
  size_t getSize() override { return 40; }
};

}; // namespace DTO