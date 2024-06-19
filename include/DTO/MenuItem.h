#pragma once

#include "FoodItem.h"
#include <cstdint>

namespace DTO {

struct MenuItem {
  uint64_t menuItemId;
  uint64_t foodItemId;
  uint64_t menuId;
  uint32_t quantity;
  MenuItem(uint64_t menuItemId, uint64_t foodItemId, uint64_t menuId,
           uint32_t quantity)
      : menuItemId(menuItemId), foodItemId(foodItemId), menuId(menuId),
        quantity(quantity) {}
};

}; // namespace DTO