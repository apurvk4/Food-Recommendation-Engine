#include "Server/DAO/UserFoodPreferenceDAO.h"
#include "Server/DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::UserFoodPreferenceDAO;

UserFoodPreferenceDAO::UserFoodPreferenceDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool UserFoodPreferenceDAO::addUserFoodPreference(uint64_t userId,
                                                  uint64_t attributeId) {

  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addUserFoodPreferenceStatement(
      connection->prepareStatement("INSERT INTO UserFoodPreference (userId, "
                                   "attributeId) VALUES (?, ?)"));
  addUserFoodPreferenceStatement->setUInt64(1, userId);
  addUserFoodPreferenceStatement->setUInt64(2, attributeId);
  return addUserFoodPreferenceStatement->executeUpdate();
}

bool UserFoodPreferenceDAO::deleteUserFoodPreference(uint64_t userId,
                                                     uint64_t attributeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteUserFoodPreferenceStatement(
      connection->prepareStatement("DELETE FROM UserFoodPreference WHERE "
                                   "userId = ? AND attributeId = ?"));
  deleteUserFoodPreferenceStatement->setUInt64(1, userId);
  deleteUserFoodPreferenceStatement->setUInt64(2, attributeId);
  return deleteUserFoodPreferenceStatement->executeUpdate();
}

bool UserFoodPreferenceDAO::updateUserFoodPreference(uint64_t userId,
                                                     uint64_t attributeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateUserFoodPreferenceStatement(
      connection->prepareStatement("UPDATE UserFoodPreference SET userId = ?, "
                                   "attributeId = ? WHERE userId = ?"));
  updateUserFoodPreferenceStatement->setUInt64(1, userId);
  updateUserFoodPreferenceStatement->setUInt64(2, attributeId);
  updateUserFoodPreferenceStatement->setUInt64(3, userId);
  return updateUserFoodPreferenceStatement->executeUpdate();
}

std::vector<uint64_t>
UserFoodPreferenceDAO::getUserFoodPreference(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getUserFoodPreferenceStatement(
      connection->prepareStatement("SELECT * FROM UserFoodPreference WHERE "
                                   "userId = ?"));
  getUserFoodPreferenceStatement->setUInt64(1, userId);
  std::unique_ptr<sql::ResultSet> resultSet(
      getUserFoodPreferenceStatement->executeQuery());
  std::vector<uint64_t> userFoodPreferences;
  while (resultSet->next()) {
    userFoodPreferences.push_back(resultSet->getUInt64("attributeId"));
  }
  return userFoodPreferences;
}