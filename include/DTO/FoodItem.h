#pragma once

#include "FoodItemType.h"
#include <cstdint>
#include <string>

namespace DTO {

struct FoodItem {
  uint64_t foodItemId;
  std::string itemName;
  double price;
  bool availabilityStatus;
  uint64_t foodItemTypeId;
  FoodItem(uint64_t foodItemId, std::string itemName, double price,
           bool availabilityStatus, uint64_t foodItemTypeId)
      : foodItemId(foodItemId), itemName(itemName), price(price),
        availabilityStatus(availabilityStatus), foodItemTypeId(foodItemTypeId) {
  }
};
}; // namespace DTO