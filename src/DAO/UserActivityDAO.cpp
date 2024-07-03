#include "DAO/UserActivityDAO.h"
#include "ActivityType.h"
#include "DbConnection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::UserActivityDAO;
using DTO::UserActivity;

UserActivityDAO::UserActivityDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool UserActivityDAO::addUserActivity(UserActivity userActivity) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "INSERT INTO UserActivity (userId, activityTypeId)"
          " VALUES (?, ?)")};
  preparedStatement->setUInt64(1, userActivity.userId);
  preparedStatement->setUInt64(2, userActivity.activityTypeId);
  return preparedStatement->executeUpdate();
}

std::vector<UserActivity> UserActivityDAO::getUserActivity(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM UserActivity WHERE userId = ?")};
  preparedStatement->setUInt64(1, userId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<UserActivity> userActivities;
  while (resultSet->next()) {
    UserActivity userActivity;
    userActivity.activityId = resultSet->getUInt64("activityId");
    userActivity.userId = resultSet->getUInt64("userId");
    userActivity.activityTypeId = resultSet->getUInt64("activityTypeId");
    userActivity.activityTime = resultSet->getUInt64("activityTime");
    userActivities.push_back(userActivity);
  }
  return userActivities;
}

std::vector<UserActivity>
UserActivityDAO::getUserActivity(uint64_t userId, uint64_t activityTypeId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement("SELECT * FROM UserActivity WHERE userId = "
                                   "? AND activityTypeId = ?")};
  preparedStatement->setUInt64(1, userId);
  preparedStatement->setUInt64(2, activityTypeId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<UserActivity> userActivities;
  while (resultSet->next()) {
    UserActivity userActivity;
    userActivity.activityId = resultSet->getUInt64("activityId");
    userActivity.userId = resultSet->getUInt64("userId");
    userActivity.activityTypeId = resultSet->getUInt64("activityTypeId");
    userActivity.activityTime = resultSet->getUInt64("activityTime");
    userActivities.push_back(userActivity);
  }
  return userActivities;
}

std::vector<UserActivity>
UserActivityDAO::getUserActivity(uint64_t userId, uint64_t from, uint64_t to) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM UserActivity WHERE userId = "
          "? AND activityTime >= ? AND activityTime <= ?")};
  preparedStatement->setUInt64(1, userId);
  preparedStatement->setUInt64(2, from);
  preparedStatement->setUInt64(3, to);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<UserActivity> userActivities;
  while (resultSet->next()) {
    UserActivity userActivity;
    userActivity.activityId = resultSet->getUInt64("activityId");
    userActivity.userId = resultSet->getUInt64("userId");
    userActivity.activityTypeId = resultSet->getUInt64("activityTypeId");
    userActivity.activityTime = resultSet->getUInt64("activityTime");
    userActivities.push_back(userActivity);
  }
  return userActivities;
}