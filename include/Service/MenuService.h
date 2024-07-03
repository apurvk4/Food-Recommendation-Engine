#pragma once

#include "Category.h"
#include "DTO/Menu.h"
#include "FoodItem.h"
#include "FoodItemDAO.h"
#include "IMenuDAO.h"
#include "MenuItem.h"
#include "MenuItemDAO.h"
#include <memory>

namespace Service {

using DAO::FoodItemDAO;
using DAO::IMenuDAO;
using DAO::MenuItemDAO;

class MenuService {
  std::shared_ptr<DAO::IMenuDAO> menuDAO;
  std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO;
  std::shared_ptr<DAO::IMenuItemDAO> menuItemDAO;

public:
  MenuService(std::shared_ptr<DAO::IMenuDAO> menuDAO,
              std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
              std::shared_ptr<DAO::IMenuItemDAO> menuItemDAO);
  uint64_t addMenu(DTO::Menu menu, std::vector<DTO::MenuItem> foodItems);
  bool deleteMenu(uint64_t menuId);
  bool updateMenu(DTO::Menu menu, std::vector<DTO::MenuItem> foodItems);
  std::pair<DTO::Menu, std::vector<DTO::MenuItem>> getMenu(uint64_t menuId);
  std::vector<std::pair<DTO::Menu, DTO::MenuItem>> getAllMenus();
  std::vector<DTO::Menu> getServeyMenu(std::string date);
  std::vector<DTO::Menu> getServeyMenu(std::string date,
                                       DTO::Category category);
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
  getMenuByDate(std::string date);
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
  getMenuByCategory(DTO::Category category, std::string date);
  uint64_t getMenuType(uint64_t menuId);
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
  getMenusByType(DTO::Category category);
  std::vector<DTO::FoodItem> getFoodItems(std::vector<DTO::MenuItem> menuItems);
  std::vector<DTO::MenuItem> getMenuItems(uint64_t menuId);
  ~MenuService() = default;
};

}; // namespace Service