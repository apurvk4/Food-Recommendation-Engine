#pragma once

#include "DAO/IMenuDAO.h"
#include "DTO/Menu.h"
#include "DTO/MenuItem.h"
#include "DbConnection.h"

#include <cstdint>
#include <vector>

namespace DAO {

class MenuDAO : public IMenuDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  MenuDAO();
  uint64_t addMenu(DTO::Menu menu) override;
  bool deleteMenu(uint64_t menuId) override;
  bool updateMenu(DTO::Menu menu) override;
  DTO::Menu getMenu(uint64_t menuId) override;
  std::vector<DTO::Menu> getAllMenus() override;
  uint64_t getMenuType(uint64_t menuId) override;
  std::vector<DTO::Menu> getMenusByType(uint64_t type) override;
};

}; // namespace DAO