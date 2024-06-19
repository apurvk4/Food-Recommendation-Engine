#include "DAO/PreferenceDAO.h"
#include "DTO/Preference.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::PreferenceDAO;
using DTO::Preference;

PreferenceDAO::PreferenceDAO() : dbConnection{DbConnection::getInstance()} {}

bool PreferenceDAO::addPreference(uint64_t userId, uint64_t foodItemId,
                                  uint64_t menuId, bool preference) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::unique_ptr<sql::PreparedStatement> addPreferenceStatement(
      connection->prepareStatement(
          "INSERT INTO Preference (userId, foodItemId, menuId, "
          "preference) VALUES (?, ?, ?, ?)"));
  addPreferenceStatement->setUInt64(1, userId);
  addPreferenceStatement->setUInt64(2, foodItemId);
  addPreferenceStatement->setUInt64(3, menuId);
  addPreferenceStatement->setBoolean(4, preference);
  return addPreferenceStatement->execute();
}

std::vector<Preference> PreferenceDAO::getPreferencesByUserId(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::unique_ptr<sql::PreparedStatement> getPreferencesStatement(
      connection->prepareStatement(
          "SELECT * FROM Preference WHERE userId = ?"));
  getPreferencesStatement->setUInt64(1, userId);
  std::unique_ptr<sql::ResultSet> preferencesResult(
      getPreferencesStatement->executeQuery());
  std::vector<Preference> preferences;
  while (preferencesResult->next()) {
    uint64_t preferenceId = preferencesResult->getUInt64("preferenceId");
    uint64_t foodItemId = preferencesResult->getUInt64("foodItemId");
    uint64_t menuId = preferencesResult->getUInt64("menuId");
    bool preference = preferencesResult->getBoolean("preference");
    std::string date = preferencesResult->getString("date");
    preferences.emplace_back(preferenceId, userId, foodItemId, menuId,
                             preference, date);
  }
  return preferences;
}

std::vector<Preference> PreferenceDAO::getPreferenceByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::unique_ptr<sql::PreparedStatement> getPreferencesStatement(
      connection->prepareStatement(
          "SELECT * FROM Preference WHERE menuId = ?"));
  getPreferencesStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> preferencesResult(
      getPreferencesStatement->executeQuery());
  std::vector<Preference> preferences;
  while (preferencesResult->next()) {
    uint64_t preferenceId = preferencesResult->getUInt64("preferenceId");
    uint64_t userId = preferencesResult->getUInt64("userId");
    uint64_t foodItemId = preferencesResult->getUInt64("foodItemId");
    bool preference = preferencesResult->getBoolean("preference");
    std::string date = preferencesResult->getString("date");
    preferences.emplace_back(preferenceId, userId, foodItemId, menuId,
                             preference, date);
  }
  return preferences;
}

std::vector<Preference>
PreferenceDAO::getPreferencesByFoodItemId(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::unique_ptr<sql::PreparedStatement> getPreferencesStatement(
      connection->prepareStatement(
          "SELECT * FROM Preference WHERE foodItemId = ?"));
  getPreferencesStatement->setUInt64(1, foodItemId);
  std::unique_ptr<sql::ResultSet> preferencesResult(
      getPreferencesStatement->executeQuery());
  std::vector<Preference> preferences;
  while (preferencesResult->next()) {
    uint64_t preferenceId = preferencesResult->getUInt64("preferenceId");
    uint64_t userId = preferencesResult->getUInt64("userId");
    uint64_t menuId = preferencesResult->getUInt64("menuId");
    bool preference = preferencesResult->getBoolean("preference");
    std::string date = preferencesResult->getString("date");
    preferences.emplace_back(preferenceId, userId, foodItemId, menuId,
                             preference, date);
  }
  return preferences;
}

std::vector<DTO::Preference>
PreferenceDAO::getPreferenceByDate(std::string date, uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::unique_ptr<sql::PreparedStatement> getPreferencesStatement(
      connection->prepareStatement(
          "SELECT * FROM Preference WHERE date = ? AND foodItemId = ?"));
  getPreferencesStatement->setString(1, date);
  getPreferencesStatement->setUInt64(2, foodItemId);
  std::unique_ptr<sql::ResultSet> preferencesResult(
      getPreferencesStatement->executeQuery());
  std::vector<Preference> preferences;
  while (preferencesResult->next()) {
    uint64_t preferenceId = preferencesResult->getUInt64("preferenceId");
    uint64_t userId = preferencesResult->getUInt64("userId");
    uint64_t menuId = preferencesResult->getUInt64("menuId");
    bool preference = preferencesResult->getBoolean("preference");
    preferences.emplace_back(preferenceId, userId, foodItemId, menuId,
                             preference, date);
  }
  return preferences;
}