#include "UserResponseDAO.h"
#include "DbConnection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

UserResponseDAO::UserResponseDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool UserResponseDAO::saveUserResponse(UserResponse response) {
  auto connection = dbConnection->getConnection();
  auto statement = connection->prepareStatement(
      "INSERT INTO userResponse (userId, foodItemId, menuId, preference) "
      "VALUES (?, ?, ?, ?)");
  statement->setUInt64(1, response.userId);
  statement->setUInt64(2, response.foodItemId);
  statement->setUInt64(3, response.menuId);
  statement->setBoolean(4, response.preference);
  return statement->execute();
}

UserResponse UserResponseDAO::getUserResponse(uint64_t responseId) {
  auto connection = dbConnection->getConnection();
  auto statement = connection->prepareStatement(
      "SELECT * FROM userResponse WHERE responseId = ?");
  statement->setUInt64(1, responseId);
  auto result = statement->executeQuery();
  if (result->next()) {
    return UserResponse(
        result->getUInt64("responseId"), result->getUInt64("userId"),
        result->getUInt64("foodItemId"), result->getUInt64("menuId"),
        result->getBoolean("preference"));
  }
  throw std::invalid_argument("No UserResponse with Id : " +
                              std::to_string(responseId));
}

std::vector<UserResponse>
UserResponseDAO::getUserResponseForAllFoodItemsInMenu(uint64_t menuId) {
  std::vector<UserResponse> responses;
  auto connection = dbConnection->getConnection();
  auto statement = connection->prepareStatement(
      "SELECT * FROM userResponse WHERE menuId = ?");
  statement->setUInt64(1, menuId);
  auto result = statement->executeQuery();
  while (result->next()) {
    uint64_t responseId = result->getUInt64("responseId");
    uint64_t userId = result->getUInt64("userId");
    uint64_t foodItemId = result->getUInt64("foodItemId");
    bool preference = result->getBoolean("preference");
    responses.emplace_back(responseId, userId, menuId, foodItemId, preference);
  }
  return responses;
}

std::vector<UserResponse>
UserResponseDAO::getUserResponseForFoodItemInMenu(uint64_t foodItemId,
                                                  uint64_t menuId) {
  std::vector<UserResponse> responses;
  auto connection = dbConnection->getConnection();
  auto statement = connection->prepareStatement(
      "SELECT * FROM userResponse WHERE foodItemId = ? AND menuId = ?");
  statement->setUInt64(1, foodItemId);
  statement->setUInt64(2, menuId);
  auto result = statement->executeQuery();
  while (result->next()) {
    uint64_t responseId = result->getUInt64("responseId");
    uint64_t userId = result->getUInt64("userId");
    bool preference = result->getBoolean("preference");
    responses.emplace_back(responseId, userId, menuId, foodItemId, preference);
  }
  return responses;
}