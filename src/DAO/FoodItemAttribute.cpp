#include "DAO/FoodItemAttributeDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::FoodItemAttributeDAO;

FoodItemAttributeDAO::FoodItemAttributeDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool FoodItemAttributeDAO::addFoodItemAttribute(uint64_t foodItemId,
                                                uint64_t attributeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addFoodItemAttributeStatement(
      connection->prepareStatement("INSERT INTO FoodItemAttribute (foodItemId, "
                                   "attributeId) VALUES (?, ?)"));
  addFoodItemAttributeStatement->setUInt64(1, foodItemId);
  addFoodItemAttributeStatement->setUInt64(2, attributeId);
  int rowsAffected = addFoodItemAttributeStatement->executeUpdate();
  connection->commit();
  connection->setAutoCommit(true);
  return rowsAffected;
}

bool FoodItemAttributeDAO::removeFoodItemAttribute(uint64_t foodItemId,
                                                   uint64_t attributeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> removeFoodItemAttributeStatement(
      connection->prepareStatement("DELETE FROM FoodItemAttribute WHERE "
                                   "foodItemId = ? AND attributeId = ?"));
  removeFoodItemAttributeStatement->setUInt64(1, foodItemId);
  removeFoodItemAttributeStatement->setUInt64(2, attributeId);
  int rowsAffected = removeFoodItemAttributeStatement->executeUpdate();
  connection->commit();
  connection->setAutoCommit(true);
  return rowsAffected;
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemAttributeDAO::getFoodItemAttributes(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFoodItemAttributesStatement(
      connection->prepareStatement("SELECT * FROM FoodItemAttribute WHERE "
                                   "foodItemId = ?"));
  getFoodItemAttributesStatement->setUInt64(1, foodItemId);
  std::unique_ptr<sql::ResultSet> resultSet(
      getFoodItemAttributesStatement->executeQuery());
  std::vector<uint64_t> foodItemAttributes;
  while (resultSet->next()) {
    uint64_t attributeId = resultSet->getUInt64("attributeId");
    foodItemAttributes.push_back(attributeId);
  }
  std::cout << "size of  foodItemAttributes: " << foodItemAttributes.size()
            << "\n";
  auto allAttributes = getAllAttributes();
  std::vector<std::pair<uint64_t, std::string>> foodItemAttributesPair;
  for (auto attributeId : foodItemAttributes) {
    for (auto attribute : allAttributes) {
      if (attributeId == attribute.first) {
        foodItemAttributesPair.push_back(attribute);
      }
    }
  }
  return foodItemAttributesPair;
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemAttributeDAO::getAllAttributes() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> getAllAttributesStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> resultSet(
      getAllAttributesStatement->executeQuery("SELECT * FROM Attribute"));
  std::vector<std::pair<uint64_t, std::string>> allAttributes;
  while (resultSet->next()) {
    uint64_t attributeId = resultSet->getUInt64("attributeId");
    std::string attributeName = resultSet->getString("attributeName");
    allAttributes.push_back(std::make_pair(attributeId, attributeName));
  }
  return allAttributes;
}

bool FoodItemAttributeDAO::addFoodItemAttribute(
    std::vector<uint64_t> attributeIds, uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addFoodItemAttributeStatement(
      connection->prepareStatement("INSERT INTO FoodItemAttribute (foodItemId, "
                                   "attributeId) VALUES (?, ?)"));
  for (auto attributeId : attributeIds) {
    addFoodItemAttributeStatement->setUInt64(1, foodItemId);
    addFoodItemAttributeStatement->setUInt64(2, attributeId);
    addFoodItemAttributeStatement->executeUpdate();
  }
  connection->commit();
  connection->setAutoCommit(true);
  return true;
}

std::vector<uint64_t>
FoodItemAttributeDAO::getFoodItemsWithGivenAttributes(uint64_t attributeIds) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement>
      getFoodItemsWithGivenAttributesStatement(
          connection->prepareStatement("SELECT * FROM FoodItemAttribute WHERE "
                                       "attributeId = ?"));
  getFoodItemsWithGivenAttributesStatement->setUInt64(1, attributeIds);
  std::unique_ptr<sql::ResultSet> resultSet(
      getFoodItemsWithGivenAttributesStatement->executeQuery());
  std::vector<uint64_t> foodItems;
  while (resultSet->next()) {
    uint64_t foodItemId = resultSet->getUInt64("foodItemId");
    foodItems.push_back(foodItemId);
  }
  return foodItems;
}