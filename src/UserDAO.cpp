#include "UserDAO.h"
#include "Roles.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

UserDAO::UserDAO() { dbConnection = DbConnection::getInstance(); }

std::vector<User> UserDAO::getAllUsers() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> resultSet(
      statement->executeQuery("SELECT * FROM Users"));
  std::vector<User> users;
  while (resultSet->next()) {
    users.push_back(User(resultSet->getInt("id"), resultSet->getString("name"),
                         static_cast<Roles>(resultSet->getInt("roleId")),
                         resultSet->getString("password")));
  }
  return users;
}

User UserDAO::getUserById(int id) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("SELECT * FROM Users WHERE id = ?"));
  preparedStatement->setInt(1, id);
  std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());
  if (resultSet->next()) {
    return User(resultSet->getInt("id"), resultSet->getString("name"),
                static_cast<Roles>(resultSet->getInt("roleId")),
                resultSet->getString("password"));
  }
  throw std::invalid_argument("User not found, with id: " + std::to_string(id));
}

bool UserDAO::addUser(User user) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("INSERT INTO Users (id, name, roleId, "
                                   "password) VALUES (?, ?, ?, ?)"));
  preparedStatement->setInt(1, user.id);
  preparedStatement->setString(2, user.name);
  preparedStatement->setInt(3, user.roleId);
  preparedStatement->setString(4, user.password);
  return preparedStatement->execute();
}

bool UserDAO::updateUser(User user) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("UPDATE Users SET name = ?, roleId = ?, "
                                   "password = ? WHERE id = ?"));
  preparedStatement->setString(1, user.name);
  preparedStatement->setInt(2, user.roleId);
  preparedStatement->setString(3, user.password);
  preparedStatement->setInt(4, user.id);
  return preparedStatement->execute();
}

bool UserDAO::deleteUser(int id) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("DELETE FROM Users WHERE id = ?"));
  preparedStatement->setInt(1, id);
  return preparedStatement->execute();
}

bool UserDAO::changePassword(int id, std::string newPassword) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> preparedStatement(
      connection->prepareStatement("UPDATE Users SET password = ? WHERE id = "
                                   "?"));
  preparedStatement->setString(1, newPassword);
  preparedStatement->setInt(2, id);
  return preparedStatement->execute();
}
