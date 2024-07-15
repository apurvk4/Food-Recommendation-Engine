#include "Server/DAO/MenuItemDAO.h"
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
      connection->prepareStatement("INSERT INTO MenuItem (foodItemId, menuId, "
                                   "quantity) VALUES (?, ?, ?)"));
  addMenuItemStatement->setUInt64(1, foodItemId);
  addMenuItemStatement->setUInt64(2, menuId);
  addMenuItemStatement->setUInt(3, quantity);
  return addMenuItemStatement->executeUpdate();
}

bool MenuItemDAO::deleteMenuItem(uint64_t menuItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteMenuItemStatement(
      connection->prepareStatement(
          "DELETE FROM MenuItem WHERE menuItemId = ?"));
  deleteMenuItemStatement->setUInt64(1, menuItemId);
  return deleteMenuItemStatement->executeUpdate();
}

bool MenuItemDAO::updateMenuItemQuantity(uint64_t menuItemId,
                                         uint32_t quantity) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateMenuItemStatement(
      connection->prepareStatement(
          "UPDATE MenuItem SET quantity = ? WHERE menuItemId = ?"));
  updateMenuItemStatement->setUInt(1, quantity);
  return updateMenuItemStatement->executeUpdate();
}

DTO::MenuItem MenuItemDAO::getMenuItemById(uint64_t menuItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getMenuItemStatement(
      connection->prepareStatement(
          "SELECT * FROM MenuItem WHERE menuItemId = ?"));
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
      connection->prepareStatement("SELECT * FROM MenuItem WHERE menuId = ?"));
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

bool MenuItemDAO::deleteMenuItemByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteMenuItemStatement(
      connection->prepareStatement("DELETE FROM MenuItem WHERE menuId = ?"));
  deleteMenuItemStatement->setUInt64(1, menuId);
  return deleteMenuItemStatement->executeUpdate();
}