#include "Service/MenuService.h"
#include "DbConnection.h"
#include "MenuItem.h"

using Service::MenuService;

MenuService::MenuService(std::shared_ptr<DAO::IMenuDAO> menuDAO,
                         std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
                         std::shared_ptr<DAO::IMenuItemDAO> menuItemDAO)
    : menuDAO(menuDAO), foodItemDAO(foodItemDAO), menuItemDAO(menuItemDAO) {}

uint64_t MenuService::addMenu(DTO::Menu menu,
                              std::vector<DTO::MenuItem> menuItems) {
  auto dbConnection = DbConnection::getInstance();
  auto connection = dbConnection->getConnection();
  connection->setAutoCommit(false);
  try {
    uint64_t menuId = menuDAO->addMenu(menu);
    for (auto &item : menuItems) {
      menuItemDAO->addMenuItem(item.foodItemId, menuId, item.quantity);
    }
    connection->commit();
    connection->setAutoCommit(true);
    return menuId;
  } catch (std::exception &e) {
    connection->rollback();
    connection->setAutoCommit(true);
    std::cout << "exception : " << e.what() << std::endl;
    throw e;
  }
}

bool MenuService::deleteMenu(uint64_t menuId) {
  auto dbConnection = DbConnection::getInstance();
  auto connection = dbConnection->getConnection();
  connection->setAutoCommit(false);
  try {
    menuItemDAO->deleteMenuItemByMenuId(menuId);
    bool result = menuDAO->deleteMenu(menuId);
    connection->commit();
    connection->setAutoCommit(true);
    return result;
  } catch (std::exception &e) {
    connection->rollback();
    connection->setAutoCommit(true);
    throw e;
  }
}

bool MenuService::updateMenu(DTO::Menu menu,
                             std::vector<DTO::MenuItem> menuItems) {
  auto dbConnection = DbConnection::getInstance();
  auto connection = dbConnection->getConnection();
  connection->setAutoCommit(false);
  try {
    menuItemDAO->deleteMenuItemByMenuId(menu.menuId);
    for (auto &item : menuItems) {
      menuItemDAO->addMenuItem(item.foodItemId, menu.menuId, item.quantity);
    }
    bool result = menuDAO->updateMenu(menu);
    connection->commit();
    connection->setAutoCommit(true);
    return result;
  } catch (std::exception &e) {
    connection->rollback();
    connection->setAutoCommit(true);
    throw;
  }
}

std::pair<DTO::Menu, std::vector<DTO::MenuItem>>
MenuService::getMenu(uint64_t menuId) {
  DTO::Menu menu = menuDAO->getMenu(menuId);
  std::vector<DTO::MenuItem> menuItems =
      menuItemDAO->getMenuItemByMenuId(menuId);
  return std::make_pair(menu, menuItems);
}

std::vector<std::pair<DTO::Menu, DTO::MenuItem>> MenuService::getAllMenus() {
  std::vector<std::pair<DTO::Menu, DTO::MenuItem>> result;
  std::vector<DTO::Menu> menus = menuDAO->getAllMenus();
  for (auto &menu : menus) {
    std::vector<DTO::MenuItem> menuItems =
        menuItemDAO->getMenuItemByMenuId(menu.menuId);
    result.push_back(std::make_pair(menu, menuItems[0]));
  }
  return result;
}

std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
MenuService::getMenuByDate(std::string date) {
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>> result;
  std::vector<DTO::Menu> menus = menuDAO->getAllMenus();
  for (auto &menu : menus) {
    std::cout << "menu date :" << (std::string)menu.date << std::endl;
    std::cout << "input date : " << date << std::endl;
    if (menu.date == date) {
      std::vector<DTO::MenuItem> menuItems =
          menuItemDAO->getMenuItemByMenuId(menu.menuId);
      result.push_back(std::make_pair(menu, menuItems));
    }
  }
  return result;
}

std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
MenuService::getMenuByCategory(DTO::Category category, std::string date) {
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>> result;
  std::vector<DTO::Menu> menus = menuDAO->getMenusByType(category);
  for (auto &menu : menus) {
    if (menu.date == date) {
      std::vector<DTO::MenuItem> menuItems =
          menuItemDAO->getMenuItemByMenuId(menu.menuId);
      result.push_back(std::make_pair(menu, menuItems));
    }
  }
  return result;
}

uint64_t MenuService::getMenuType(uint64_t menuId) {
  return menuDAO->getMenuType(menuId);
}

std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
MenuService::getMenusByType(DTO::Category category) {
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>> result;
  std::vector<DTO::Menu> menus = menuDAO->getMenusByType(category);
  for (auto &menu : menus) {
    std::vector<DTO::MenuItem> menuItems =
        menuItemDAO->getMenuItemByMenuId(menu.menuId);
    result.push_back(std::make_pair(menu, menuItems));
  }
  return result;
}

std::vector<DTO::FoodItem>
MenuService::getFoodItems(std::vector<DTO::MenuItem> menuItems) {
  std::vector<DTO::FoodItem> result;
  for (auto &item : menuItems) {
    result.push_back(foodItemDAO->getFoodItemById(item.foodItemId));
  }
  return result;
}

std::vector<DTO::MenuItem> MenuService::getMenuItems(uint64_t menuId) {
  return menuItemDAO->getMenuItemByMenuId(menuId);
}

std::vector<DTO::Menu> MenuService::getServeyMenu(std::string date) {
  return menuDAO->getSurveyMenu(date);
}

std::vector<DTO::Menu> MenuService::getServeyMenu(std::string date,
                                                  DTO::Category category) {
  return menuDAO->getSurveyMenu(date, category);
}