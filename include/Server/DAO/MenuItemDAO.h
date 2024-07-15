#pragma once

#include "DTO/MenuItem.h"
#include "IMenuItemDAO.h"
#include "Server/DbConnection.h"
#include <cstdint>
#include <string>
#include <vector>


namespace DAO {

class MenuItemDAO : public IMenuItemDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  MenuItemDAO();
  bool addMenuItem(uint64_t foodItemId, uint64_t menuId,
                   uint32_t quantity) override;
  bool deleteMenuItem(uint64_t menuItemId) override;
  bool deleteMenuItemByMenuId(uint64_t menuId) override;
  bool updateMenuItemQuantity(uint64_t menuItemId, uint32_t quantity) override;
  DTO::MenuItem getMenuItemById(uint64_t menuItemId) override;
  std::vector<DTO::MenuItem> getMenuItemByMenuId(uint64_t menuId) override;
};

}; // namespace DAO