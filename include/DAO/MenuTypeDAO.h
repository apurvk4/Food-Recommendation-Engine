#pragma once

#include "DAO/IMenuDAO.h"
#include "DAO/IMenuTypeDAO.h"
#include "DTO/MenuType.h"
#include "DbConnection.h"
#include <cstdint>
#include <string>

namespace DAO {

class MenuTypeDAO : public IMenuTypeDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  MenuTypeDAO();
  std::vector<MenuType> getAllMenuTypes() override;
  MenuType getMenuTypeById(uint64_t menuTypeId) override;
  MenuType getMenuTypeByName(std::string menuType) override;
  void addMenuType(MenuType menuType) override;
  void updateMenuType(MenuType menuType) override;
  void deleteMenuType(uint64_t menuTypeId) override;
};
}; // namespace DAO