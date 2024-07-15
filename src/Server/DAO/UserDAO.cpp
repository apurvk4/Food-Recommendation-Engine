#include "Server/DAO/UserDAO.h"
#include "DTO/User.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::UserDAO;
using DTO::User;

UserDAO::UserDAO() { dbConnection = DbConnection::getInstance(); }

std::vector<User> UserDAO::getAllUsers() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> resultSet(
      statement->executeQuery("SELECT * FROM User"));
  std::vector<User> users;
  while (resultSet->next()) {
    uint64_t userId = resultSet->getUInt64("userId");
    std::string userName = resultSet->getString("name");
    uint64_t roleId = resultSet->getUInt64("roleId");
    std::string password = resultSet->getString("password");
    uint64_t lastNotificationId = resultSet->getUInt64("lastNotificationId");
    users.emplace_back(userId, userName, password, roleId, lastNotificationId);
  }
  return users;
}

User UserDAO::getUserById(int userId) {
  if (dbConnection == nullptr) {
    throw std::invalid_argument("Connection is not established");
  }
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  if (connection == nullptr) {
    throw std::invalid_argument("Connection is not established");
  }
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("SELECT * FROM User WHERE userId = ?"));
  preparedStatement->setInt(1, userId);
  std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());
  if (resultSet->next()) {
    std::string userName = resultSet->getString("name");
    uint64_t roleId = resultSet->getUInt64("roleId");
    std::string password = resultSet->getString("password");
    uint64_t lastNotificationId = resultSet->getUInt64("lastNotificationId");
    return User(userId, userName, password, roleId, lastNotificationId);
  }
  std::cout << "User Not found\n";
  throw std::invalid_argument("Invalid userId, cannot find user with given "
                              "userId : " +
                              std::to_string(userId));
}

bool UserDAO::addUser(User user) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("INSERT INTO User (name, roleId, "
                                   "password) VALUES (?, ?, ?)"));
  preparedStatement->setString(1, (std::string)user.name);
  preparedStatement->setInt(2, user.roleId);
  preparedStatement->setString(3, (std::string)user.password);
  return preparedStatement->executeUpdate();
}

bool UserDAO::updateUser(User user) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("UPDATE User SET name = ?, roleId = ?, "
                                   "lastNotificationId = ? WHERE userId = ?"));
  preparedStatement->setString(1, (std::string)user.name);
  preparedStatement->setInt(2, user.roleId);
  preparedStatement->setInt(3, user.lastNotificationId);
  preparedStatement->setInt(4, user.userId);
  return preparedStatement->executeUpdate();
}

bool UserDAO::deleteUser(int id) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("DELETE FROM User WHERE userId = ?"));
  preparedStatement->setInt(1, id);
  return preparedStatement->executeUpdate();
}

bool UserDAO::changePassword(int id, std::string newPassword) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement(
          "UPDATE User SET password = ? WHERE userId = ?"));
  preparedStatement->setString(1, newPassword);
  preparedStatement->setInt(2, id);
  return preparedStatement->executeUpdate();
}

uint64_t UserDAO::getLastNotificationId(int userId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement(
          "SELECT lastNotificationId FROM User WHERE userId = ?"));
  preparedStatement->setInt(1, userId);
  std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());
  if (resultSet->next()) {
    uint64_t lastNotificationId = resultSet->getUInt64("lastNotificationId");
    return lastNotificationId;
  }
  throw std::invalid_argument("Invalid userId, cannot find user with given "
                              "userId : " +
                              std::to_string(userId));
}
