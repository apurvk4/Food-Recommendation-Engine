#include "DAO/FoodItemTypeDAO.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using ::DAO::FoodItemTypeDAO;

FoodItemTypeDAO::FoodItemTypeDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool FoodItemTypeDAO::addFoodItemType(std::string foodItemType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "INSERT INTO foodItemType (foodItemType) VALUES ('" +
                      foodItemType + "');";
  std::unique_ptr<sql::Statement> addFoodItemTypeStatement(
      connection->createStatement());
  return addFoodItemTypeStatement->execute(query);
}

bool FoodItemTypeDAO::deleteFoodItemType(uint64_t foodItemTypeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "DELETE FROM foodItemType WHERE foodItemTypeId = " +
                      std::to_string(foodItemTypeId) + ";";
  std::unique_ptr<sql::Statement> deleteFoodItemTypeStatement(
      connection->createStatement());
  return deleteFoodItemTypeStatement->execute(query);
}

bool FoodItemTypeDAO::updateFoodItemType(uint64_t foodItemTypeId,
                                         std::string foodItemType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query =
      "UPDATE foodItemType SET foodItemType = '" + foodItemType +
      "' WHERE foodItemTypeId = " + std::to_string(foodItemTypeId) + ";";
  std::unique_ptr<sql::Statement> updateFoodItemTypeStatement(
      connection->createStatement());
  return updateFoodItemTypeStatement->execute(query);
}

std::string FoodItemTypeDAO::getFoodItemTypeById(uint64_t foodItemTypeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "SELECT * FROM foodItemType WHERE foodItemTypeId = " +
                      std::to_string(foodItemTypeId) + ";";
  std::unique_ptr<sql::Statement> getFoodItemTypeStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> foodItemTypeResult(
      getFoodItemTypeStatement->executeQuery(query));
  if (foodItemTypeResult->next()) {
    return foodItemTypeResult->getString("foodItemType");
  }
  throw std::invalid_argument("Invalid foodItemTypeId, cannot find "
                              "foodItemType with given foodItemTypeId : " +
                              std::to_string(foodItemTypeId));
}

std::string FoodItemTypeDAO::getFoodItemTypeByName(std::string foodItemType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query =
      "SELECT * FROM foodItemType WHERE foodItemType = '" + foodItemType + "';";
  std::unique_ptr<sql::Statement> getFoodItemTypeStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> foodItemTypeResult(
      getFoodItemTypeStatement->executeQuery(query));
  if (foodItemTypeResult->next()) {
    return foodItemTypeResult->getString("foodItemType");
  }
  throw std::invalid_argument("Invalid foodItemType, cannot find "
                              "foodItemType with given foodItemType : " +
                              foodItemType);
}