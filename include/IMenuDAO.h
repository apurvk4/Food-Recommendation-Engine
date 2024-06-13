#pragma once

#include "Menu.h"
#include <vector>

enum MenuType { Breakfast = 1, Lunch = 2, Dinner = 3 };

class IMenuDAO {
public:
  virtual bool addMenu(Menu menu) = 0;
  virtual bool deleteMenu(uint64_t menuId) = 0;
  virtual bool updateMenu(Menu menu) = 0;
  virtual Menu getMenu(uint64_t menuId) = 0;
  virtual std::vector<Menu> getAllMenus() = 0;
  virtual std::vector<Menu> getMenusByType(MenuType type) = 0;
  virtual std::vector<Menu> getMenusByDate(std::string date) = 0;
  virtual ~IMenuDAO() = default;
};