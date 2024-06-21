#include "DAO/MenuDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::MenuDAO;

MenuDAO::MenuDAO() : dbConnection{DbConnection::getInstance()} {}

uint64_t MenuDAO::addMenu(DTO::Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "INSERT INTO Menu (menuName, menuTypeId) VALUES (?, ?)"));
  prepStmt->setString(1, (std::string)menu.menuName);
  prepStmt->setUInt64(2, menu.menuTypeId);
  prepStmt->execute();
  // get last inserted id
  std::shared_ptr<sql::Statement> stmt;
  stmt = std::shared_ptr<sql::Statement>(connection->createStatement());
  std::shared_ptr<sql::ResultSet> menuResult(
      stmt->executeQuery("SELECT LAST_INSERT_ID() as id"));
  if (menuResult->next()) {
    return menuResult->getUInt64("id");
  }
  throw std::runtime_error("Failed to get last inserted id");
}

bool MenuDAO::deleteMenu(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("DELETE FROM Menu WHERE menuId = ?"));
  prepStmt->setUInt64(1, menuId);
  return prepStmt->execute();
}

bool MenuDAO::updateMenu(DTO::Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "UPDATE Menu SET menuName = ?, menuTypeId = ? "
          "WHERE menuId = ?"));
  prepStmt->setString(1, (std::string)menu.menuName);
  prepStmt->setUInt64(2, menu.menuTypeId);
  prepStmt->setUInt64(3, menu.menuId);
  return prepStmt->execute();
}

DTO::Menu MenuDAO::getMenu(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE menuId = ?"));
  prepStmt->setUInt64(1, menuId);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  if (menuResult->next()) {
    menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    uint64_t menuTypeId = menuResult->getUInt64("menuTypeId");
    return DTO::Menu{menuId, menuName, menuTypeId};
  }
  throw std::invalid_argument("Invalid menuId, cannot find menu with given "
                              "menuId : " +
                              std::to_string(menuId));
}

std::vector<DTO::Menu> MenuDAO::getAllMenus() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::Statement> stmt;
  stmt = std::shared_ptr<sql::Statement>(connection->createStatement());
  std::shared_ptr<sql::ResultSet> menuResult(
      stmt->executeQuery("SELECT * FROM Menu"));
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    uint64_t menuTypeId = menuResult->getUInt64("menuTypeId");
    menus.emplace_back(menuId, menuName, menuTypeId);
  }
  return menus;
}

std::vector<DTO::Menu> MenuDAO::getMenusByType(uint64_t menuTypeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE menuTypeId = ?"));
  prepStmt->setUInt64(1, menuTypeId);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    menuTypeId = menuResult->getUInt64("menuTypeId");
    menus.emplace_back(menuId, menuName, menuTypeId);
  }
  return menus;
}

uint64_t MenuDAO::getMenuType(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "SELECT menuTypeId FROM Menu WHERE menuId = ?"));
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  if (menuResult->next()) {
    uint64_t menuTypeId = menuResult->getUInt64("menuTypeId");
    return menuTypeId;
  }
  throw std::runtime_error("invalid menuId, menuId : " +
                           std::to_string(menuId));
}