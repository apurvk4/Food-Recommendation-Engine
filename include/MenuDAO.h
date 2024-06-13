#pragma once

#include "DbConnection.h"
#include "FoodItem.h"
#include "IMenuDAO.h"
#include <cstdint>

class MenuDAO : public IMenuDAO {
  std::shared_ptr<DbConnection> dbConnection;
  std::vector<FoodItem> getAllFoodItems(uint64_t menuId);

public:
  MenuDAO();
  bool addMenu(Menu menu) override;
  bool deleteMenu(uint64_t menuId) override;
  bool updateMenu(Menu menu) override;
  Menu getMenu(uint64_t menuId) override;
  std::vector<Menu> getAllMenus() override;
  std::vector<Menu> getMenusByType(MenuType type) override;
  std::vector<Menu> getMenusByDate(std::string date) override;
};