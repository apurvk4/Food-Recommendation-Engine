#pragma once

#include "FoodItem.h"
#include "MenuItem.h"
#include "MenuType.h"
#include <string>
#include <vector>

namespace DTO {

struct Menu {
  uint64_t menuId;
  std::string menuName;
  uint64_t menuTypeId;
  Menu(uint64_t menuId, std::string menuName, uint64_t menuTypeId)
      : menuId(menuId), menuName(menuName), menuTypeId(menuTypeId) {}
};

}; // namespace DTO