#pragma once

#include "DTO/MenuType.h"
#include <cstdint>
#include <string>
#include <vector>

namespace DAO {

using DTO::MenuType;

class IMenuTypeDAO {
public:
  virtual ~IMenuTypeDAO() = default;
  virtual std::vector<MenuType> getAllMenuTypes() = 0;
  virtual MenuType getMenuTypeById(uint64_t menuTypeId) = 0;
  virtual MenuType getMenuTypeByName(std::string menuType) = 0;
  virtual void addMenuType(MenuType menuType) = 0;
  virtual void updateMenuType(MenuType menuType) = 0;
  virtual void deleteMenuType(uint64_t menuTypeId) = 0;
};
}; // namespace DAO