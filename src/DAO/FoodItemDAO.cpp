#include "DAO/FoodItemDAO.h"
#include "DTO/FoodItem.h"
#include "DTO/FoodItemType.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <memory>
#include <stdexcept>
#include <string>

using ::DAO::FoodItemDAO;
using ::DTO::FoodItem;
using ::DTO::FoodItemType;

FoodItemDAO::FoodItemDAO() : dbConnection{DbConnection::getInstance()} {}

bool FoodItemDAO::addFoodItem(FoodItem foodItem) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addFoodItemStatement(
      connection->prepareStatement(
          "INSERT INTO FoodItem (itemName, "
          "price,availabilityStatus,foodItemTypeId) VALUES (?, ?, ?, ?)"));
  addFoodItemStatement->setString(1, (std::string)foodItem.itemName);
  addFoodItemStatement->setDouble(2, foodItem.price);
  addFoodItemStatement->setBoolean(3, foodItem.availabilityStatus);
  addFoodItemStatement->setUInt64(4, foodItem.foodItemTypeId);
  return addFoodItemStatement->execute();
}

bool FoodItemDAO::deleteFoodItem(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteFoodItemStatement(
      connection->prepareStatement(
          "DELETE FROM FoodItem WHERE foodItemId = ?"));
  deleteFoodItemStatement->setUInt64(1, foodItemId);
  return deleteFoodItemStatement->execute();
}

bool FoodItemDAO::updateFoodItem(FoodItem foodItem) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateFoodItemStatement(
      connection->prepareStatement("UPDATE FoodItem SET itemName = ?, price = "
                                   "?, availabilityStatus = ?, foodItemTypeId "
                                   "= ? WHERE foodItemId = ?"));
  updateFoodItemStatement->setString(1, (std::string)foodItem.itemName);
  updateFoodItemStatement->setDouble(2, foodItem.price);
  updateFoodItemStatement->setBoolean(3, foodItem.availabilityStatus);
  updateFoodItemStatement->setUInt(4, foodItem.foodItemTypeId);
  updateFoodItemStatement->setUInt64(5, foodItem.foodItemId);
  return updateFoodItemStatement->execute();
}

FoodItem FoodItemDAO::getFoodItemById(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFoodItemStatement(
      connection->prepareStatement(
          "SELECT * FROM FoodItem WHERE foodItemId = ?"));
  getFoodItemStatement->setUInt64(1, foodItemId);
  std::unique_ptr<sql::ResultSet> foodItemResult(
      getFoodItemStatement->executeQuery());
  if (foodItemResult->next()) {
    std::string itemName = foodItemResult->getString("itemName");
    double price = foodItemResult->getDouble("price");
    bool availabilityStatus = foodItemResult->getBoolean("availabilityStatus");
    uint64_t foodItemTypeId = foodItemResult->getUInt64("foodItemTypeId");
    return FoodItem(foodItemId, price, availabilityStatus, foodItemTypeId,
                    itemName);
  }
  throw std::invalid_argument("No Food items with Id : " +
                              std::to_string(foodItemId));
}

std::vector<FoodItem> FoodItemDAO::getFoodItemsByPrice(double minPrice,
                                                       double maxPrice) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFoodItemsStatement(
      connection->prepareStatement(
          "SELECT * FROM FoodItem WHERE price >= ? AND price <= ?"));
  getFoodItemsStatement->setDouble(1, minPrice);
  getFoodItemsStatement->setDouble(2, maxPrice);
  std::unique_ptr<sql::ResultSet> foodItemsResult(
      getFoodItemsStatement->executeQuery());
  std::vector<FoodItem> foodItems;
  while (foodItemsResult->next()) {
    uint64_t foodItemId = foodItemsResult->getUInt64("foodItemId");
    std::string itemName = foodItemsResult->getString("itemName");
    double price = foodItemsResult->getDouble("price");
    bool availabilityStatus = foodItemsResult->getBoolean("availabilityStatus");
    uint64_t foodItemType = foodItemsResult->getUInt64("foodItemTypeId");
    foodItems.emplace_back(foodItemId, price, availabilityStatus, foodItemType,
                           itemName);
  }
  return foodItems;
}

std::vector<FoodItem> FoodItemDAO::getAvailableFoodItems() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getAvailableFoodItemsStatement(
      connection->prepareStatement(
          "SELECT * FROM FoodItem WHERE availabilityStatus = 1"));
  std::unique_ptr<sql::ResultSet> foodItemsResult(
      getAvailableFoodItemsStatement->executeQuery());
  std::vector<FoodItem> foodItems;
  while (foodItemsResult->next()) {
    uint64_t foodItemId = foodItemsResult->getUInt64("foodItemId");
    std::string itemName = foodItemsResult->getString("itemName");
    double price = foodItemsResult->getDouble("price");
    bool availabilityStatus = foodItemsResult->getBoolean("availabilityStatus");
    uint64_t foodItemType = foodItemsResult->getUInt64("foodItemTypeId");
    foodItems.emplace_back(foodItemId, price, availabilityStatus, foodItemType,
                           itemName);
  }
  return foodItems;
}

std::vector<FoodItem> FoodItemDAO::getAllFoodItems() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> foodItemsResult(
      statement->executeQuery("SELECT * FROM FoodItem"));
  std::vector<FoodItem> foodItems;
  while (foodItemsResult->next()) {
    uint64_t foodItemId = foodItemsResult->getUInt64("foodItemId");
    std::string itemName = foodItemsResult->getString("itemName");
    double price = foodItemsResult->getDouble("price");
    bool availabilityStatus = foodItemsResult->getBoolean("availabilityStatus");
    uint64_t foodItemTypeId = foodItemsResult->getUInt64("foodItemTypeId");
    foodItems.emplace_back(foodItemId, price, availabilityStatus,
                           foodItemTypeId, itemName);
  }
  std::cout << "Returning foodItems\n";
  return foodItems;
}

std::vector<FoodItem>
FoodItemDAO::getFoodItemsByType(FoodItemType foodItemType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFoodItemsByTypeStatement(
      connection->prepareStatement(
          "SELECT * FROM FoodItem WHERE foodItemTypeId = ?"));
  getFoodItemsByTypeStatement->setUInt64(1, foodItemType.foodItemTypeId);
  std::unique_ptr<sql::ResultSet> foodItemsResult(
      getFoodItemsByTypeStatement->executeQuery());
  std::vector<FoodItem> foodItems;
  while (foodItemsResult->next()) {
    uint64_t foodItemId = foodItemsResult->getUInt64("foodItemId");
    std::string itemName = foodItemsResult->getString("itemName");
    double price = foodItemsResult->getDouble("price");
    bool availabilityStatus = foodItemsResult->getBoolean("availabilityStatus");
    foodItems.emplace_back(foodItemId, price, availabilityStatus,
                           foodItemType.foodItemTypeId, itemName);
  }
  return foodItems;
}