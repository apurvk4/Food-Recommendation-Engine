#include "MenuDAO.h"
#include "Menu.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdint>
#include <exception>
#include <memory>
#include <stdexcept>

MenuDAO::MenuDAO() { dbConnection = DbConnection::getInstance(); }

std::vector<FoodItem> MenuDAO::getAllFoodItems(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> foodItemStatement(
      connection->prepareStatement(
          "SELECT * FROM FoodItemsInMenu WHERE menuId = ?"));
  foodItemStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> foodItemsResult(
      foodItemStatement->executeQuery());

  std::vector<FoodItem> foodItems;
  while (foodItemsResult->next()) {
    int foodItemId = foodItemsResult->getUInt64("foodItemId");
    std::string name = foodItemsResult->getString("name");
    double price = foodItemsResult->getDouble("price");
    int menuId = foodItemsResult->getInt("menuId");

    foodItems.emplace_back(foodItemId, name, price, menuId);
  }
  return foodItems;
}

bool MenuDAO::addMenu(Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  try {
    connection->setAutoCommit(false);

    std::unique_ptr<sql::PreparedStatement> menuStatement(
        connection->prepareStatement(
            "INSERT INTO Menus (date, MenuTypeId) VALUES (?, ?)"));
    menuStatement->setString(1, menu.date);
    menuStatement->setInt(2, menu.MenuTypeId);
    if (!menuStatement->execute()) {
      throw std::runtime_error("Failed to insert Menu");
    }
    std::unique_ptr<sql::PreparedStatement> idStatement(
        connection->prepareStatement("SELECT LAST_INSERT_ID()"));
    std::unique_ptr<sql::ResultSet> generatedKeys(idStatement->executeQuery());
    int menuId;
    if (generatedKeys->next()) {
      menuId = generatedKeys->getInt64(1);
    } else {
      throw std::runtime_error("Failed to retrieve generated key for Menu");
    }
    std::unique_ptr<sql::PreparedStatement> foodItemStatement(
        connection->prepareStatement("INSERT INTO FoodItemsInMenu foodItemId, "
                                     "menuId) VALUES (?, ?)"));
    for (const auto &foodItem : menu.foodItems) {
      foodItemStatement->setUInt64(1, foodItem.foodItemId);
      foodItemStatement->setInt(2, menuId);
      foodItemStatement->execute();
    }

    connection->commit();
    connection->setAutoCommit(true);

    return true;
  } catch (const std::exception &e) {
    connection->rollback();
    connection->setAutoCommit(true);
    return false;
  }
}

Menu MenuDAO::getMenu(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();

  // Retrieve the menu details
  std::unique_ptr<sql::PreparedStatement> menuStatement(
      connection->prepareStatement("SELECT * FROM Menus WHERE menuId = ?"));
  menuStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> menuResult(menuStatement->executeQuery());

  if (!menuResult->next()) {
    throw std::invalid_argument("Could not find menu with menuId: " +
                                std::to_string(menuId));
  }
  auto foodItems = getAllFoodItems(menuId);
  std::string date = menuResult->getString("date");
  int menuTypeId = menuResult->getInt("MenuTypeId");
  return Menu(menuId, date, menuTypeId, foodItems);
}

bool MenuDAO::deleteMenu(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteMenuStatement(
      connection->prepareStatement("DELETE FROM Menus WHERE menuId = ?"));
  deleteMenuStatement->setUInt64(1, menuId);
  return deleteMenuStatement->execute();
}

std::vector<Menu> MenuDAO::getMenusByType(MenuType type) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();

  std::unique_ptr<sql::PreparedStatement> getMenusStatement(
      connection->prepareStatement("SELECT * FROM Menus WHERE menuTypeId = ?"));
  getMenusStatement->setUInt64(1, static_cast<uint64_t>(type));

  std::unique_ptr<sql::ResultSet> menusResult(
      getMenusStatement->executeQuery());

  std::vector<Menu> result;
  while (menusResult->next()) {
    uint64_t menuId = menusResult->getUInt64("menuId");
    uint64_t menuTypeId = menusResult->getUInt64("menuTypeId");
    std::string date = menusResult->getString("date");
    auto foodItems = getAllFoodItems(menuId);
    result.emplace_back(Menu(menuId, date, menuTypeId, foodItems));
  }
  return result;
}

std::vector<Menu> MenuDAO::getAllMenus() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();

  std::unique_ptr<sql::PreparedStatement> getMenusStatement(
      connection->prepareStatement("SELECT * FROM Menus"));

  std::unique_ptr<sql::ResultSet> menusResult(
      getMenusStatement->executeQuery());

  std::vector<Menu> result;
  while (menusResult->next()) {
    uint64_t menuId = menusResult->getUInt64("menuId");
    uint64_t menuTypeId = menusResult->getUInt64("menuTypeId");
    std::string date = menusResult->getString("date");
    auto foodItems = getAllFoodItems(menuId);
    result.emplace_back(Menu(menuId, date, menuTypeId, foodItems));
  }
  return result;
}

bool MenuDAO::updateMenu(Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  try {
    connection->setAutoCommit(false);

    std::unique_ptr<sql::PreparedStatement> menuStatement(
        connection->prepareStatement(
            "UPDATE Menus SET date = ?, MenuTypeId = ? WHERE menuId = ?"));
    menuStatement->setString(1, menu.date);
    menuStatement->setInt(2, menu.MenuTypeId);
    menuStatement->setUInt64(3, menu.menuId);
    if (!menuStatement->execute()) {
      throw std::runtime_error("Failed to update Menu");
    }

    std::unique_ptr<sql::PreparedStatement> deleteFoodItemsStatement(
        connection->prepareStatement(
            "DELETE FROM FoodItemsInMenu WHERE menuId = ?"));
    deleteFoodItemsStatement->setUInt64(1, menu.menuId);
    deleteFoodItemsStatement->execute();

    std::unique_ptr<sql::PreparedStatement> foodItemStatement(
        connection->prepareStatement("INSERT INTO FoodItemsInMenu (foodItemId, "
                                     "menuId) VALUES (?, ?)"));
    for (const auto &foodItem : menu.foodItems) {
      foodItemStatement->setUInt64(1, foodItem.foodItemId);
      foodItemStatement->setUInt64(2, menu.menuId);
      foodItemStatement->execute();
    }

    connection->commit();
    connection->setAutoCommit(true);

    return true;
  } catch (const std::exception &e) {
    connection->rollback();
    connection->setAutoCommit(true);
    return false;
  }
}

std::vector<Menu> MenuDAO::getMenusByDate(std::string date) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();

  std::unique_ptr<sql::PreparedStatement> getMenusStatement(
      connection->prepareStatement("SELECT * FROM Menus WHERE date = ?"));
  getMenusStatement->setString(1, date);

  std::unique_ptr<sql::ResultSet> menusResult(
      getMenusStatement->executeQuery());

  std::vector<Menu> result;
  while (menusResult->next()) {
    uint64_t menuId = menusResult->getUInt64("menuId");
    uint64_t menuTypeId = menusResult->getUInt64("menuTypeId");
    auto foodItems = getAllFoodItems(menuId);
    result.emplace_back(Menu(menuId, date, menuTypeId, foodItems));
  }
  return result;
}