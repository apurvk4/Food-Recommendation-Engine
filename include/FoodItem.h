#pragma once

#include <cstdint>
#include <string>

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