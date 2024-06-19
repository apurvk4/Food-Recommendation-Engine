#pragma once

#include <cstdint>
#include <string>

namespace DTO {

struct FoodItemType {
  uint64_t foodItemTypeId;
  std::string foodItemType;
  FoodItemType(uint64_t foodItemTypeId, std::string foodItemType)
      : foodItemTypeId(foodItemTypeId), foodItemType(foodItemType) {}
};

}; // namespace DTO