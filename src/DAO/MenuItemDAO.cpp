#include "DAO/MenuItemDAO.h"
#include "DTO/MenuItem.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdint>
#include <memory>

using DAO::MenuItemDAO;

MenuItemDAO::MenuItemDAO() : dbConnection{DbConnection::getInstance()} {}

bool MenuItemDAO::addMenuItem(uint64_t foodItemId, uint64_t menuId,
                              uint32_t quantity) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addMenuItemStatement(
      connection->prepareStatement("INSERT INTO menuItems (foodItemId, menuId, "
                                   "quantity) VALUES (?, ?, ?)"));
  addMenuItemStatement->setUInt64(1, foodItemId);
  addMenuItemStatement->setUInt64(2, menuId);
  addMenuItemStatement->setUInt(3, quantity);
  return addMenuItemStatement->execute();
}

bool MenuItemDAO::deleteMenuItem(uint64_t menuItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteMenuItemStatement(
      connection->prepareStatement(
          "DELETE FROM menuItems WHERE menuItemId = ?"));
  deleteMenuItemStatement->setUInt64(1, menuItemId);
  return deleteMenuItemStatement->execute();
}

bool MenuItemDAO::updateMenuItemQuantity(uint64_t menuItemId,
                                         uint32_t quantity) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateMenuItemStatement(
      connection->prepareStatement(
          "UPDATE menuItems SET quantity = ? WHERE menuItemId = ?"));
  updateMenuItemStatement->setUInt(1, quantity);
  return updateMenuItemStatement->execute();
}

DTO::MenuItem MenuItemDAO::getMenuItemById(uint64_t menuItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getMenuItemStatement(
      connection->prepareStatement(
          "SELECT * FROM menuItems WHERE menuItemId = ?"));
  getMenuItemStatement->setUInt64(1, menuItemId);
  std::unique_ptr<sql::ResultSet> menuItemResult(
      getMenuItemStatement->executeQuery());
  menuItemId = menuItemResult->getUInt64("menuItemId");
  uint64_t foodItemId = menuItemResult->getUInt64("foodItemId");
  uint64_t menuId = menuItemResult->getUInt64("menuId");
  uint32_t quantity = menuItemResult->getUInt("quantity");
  return DTO::MenuItem{menuItemId, foodItemId, menuId, quantity};
}

std::vector<DTO::MenuItem> MenuItemDAO::getMenuItemByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getMenuItemStatement(
      connection->prepareStatement("SELECT * FROM menuItems WHERE menuId = ?"));
  getMenuItemStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> menuItemResult(
      getMenuItemStatement->executeQuery());
  std::vector<DTO::MenuItem> menuItems;
  while (menuItemResult->next()) {
    uint64_t menuItemId = menuItemResult->getUInt64("menuItemId");
    uint64_t foodItemId = menuItemResult->getUInt64("foodItemId");
    menuId = menuItemResult->getUInt64("menuId");
    uint32_t quantity = menuItemResult->getUInt("quantity");
    menuItems.emplace_back(menuItemId, foodItemId, menuId, quantity);
  }
  return menuItems;
}
