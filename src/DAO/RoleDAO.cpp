#include "DAO/RoleDAO.h"
#include "DTO/Role.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::RoleDAO;
using DTO::Role;

RoleDAO::RoleDAO() : dbConnection(DbConnection::getInstance()) {}

bool RoleDAO::createRole(const std::string &roleName) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query =
      "INSERT INTO Role (roleName) VALUES ('" + roleName + "');";
  std::unique_ptr<sql::Statement> createRoleStatement(
      connection->createStatement());
  return createRoleStatement->execute(query);
}

bool RoleDAO::updateRole(uint64_t roleId, std::string newroleName) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "UPDATE Role SET roleName = '" + newroleName +
                      "' WHERE roleId = " + std::to_string(roleId) + ";";
  std::unique_ptr<sql::Statement> updateRoleStatement(
      connection->createStatement());
  return updateRoleStatement->execute(query);
}

std::string RoleDAO::getRoleName(uint64_t roleId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query =
      "SELECT * FROM Role WHERE roleId = " + std::to_string(roleId) + ";";
  std::unique_ptr<sql::Statement> getRoleNameStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> roleNameResult(
      getRoleNameStatement->executeQuery(query));
  if (roleNameResult->next()) {
    return roleNameResult->getString("roleName");
  }
  throw std::invalid_argument("Invalid roleId, cannot find role with given "
                              "roleId : " +
                              std::to_string(roleId));
}

uint64_t RoleDAO::getRoleId(std::string roleName) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "SELECT * FROM Role WHERE roleName = '" + roleName + "';";
  std::unique_ptr<sql::Statement> getRoleIdStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> roleIdResult(
      getRoleIdStatement->executeQuery(query));
  if (roleIdResult->next()) {
    return roleIdResult->getUInt64("roleId");
  }
  throw std::invalid_argument("Invalid roleName, cannot find role with given "
                              "roleName : " +
                              roleName);
}

std::vector<Role> RoleDAO::getRoles() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::string query = "SELECT * FROM Role;";
  std::unique_ptr<sql::Statement> getRolesStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> rolesResult(
      getRolesStatement->executeQuery(query));
  std::vector<Role> roles;
  while (rolesResult->next()) {
    uint64_t roleId = rolesResult->getUInt64("roleId");
    std::string roleName = rolesResult->getString("roleName");
    roles.emplace_back(roleId, roleName);
  }
  return roles;
}
