#pragma once

#include "FoodItem.h"
#include <string>
#include <vector>

struct Menu {
  uint64_t menuId;
  std::string menuName;
  uint64_t menuTypeId;
  std::vector<FoodItem> foodItems;
  Menu(uint64_t menuId, std::string menuName, uint64_t menuTypeId,
       std::vector<FoodItem> foodItems)
      : menuId(menuId), menuName(menuName), menuTypeId(menuTypeId),
        foodItems(foodItems) {}
};