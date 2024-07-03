#pragma once

#include "DTO/MenuItem.h"
#include <cstdint>
#include <string>
#include <vector>

namespace DAO {

class IMenuItemDAO {
public:
  virtual bool addMenuItem(uint64_t foodItemId, uint64_t menuId,
                           uint32_t quantity) = 0;
  virtual bool deleteMenuItem(uint64_t menuItemId) = 0;
  virtual bool deleteMenuItemByMenuId(uint64_t menuId) = 0;
  virtual bool updateMenuItemQuantity(uint64_t menuItemId,
                                      uint32_t quantity) = 0;
  virtual DTO::MenuItem getMenuItemById(uint64_t menuItemId) = 0;
  virtual std::vector<DTO::MenuItem> getMenuItemByMenuId(uint64_t menuId) = 0;
  virtual ~IMenuItemDAO() = default;
};

}; // namespace DAO