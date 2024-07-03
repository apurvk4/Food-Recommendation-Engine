#include "DAO/MenuDAO.h"
#include "Category.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdint>

using DAO::MenuDAO;

MenuDAO::MenuDAO() : dbConnection{DbConnection::getInstance()} {}

uint64_t MenuDAO::addMenu(DTO::Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  if (menu.date != (std::string) "") {
    prepStmt =
        std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
            "INSERT INTO Menu (menuName, categoryId,isSurvey,date) VALUES (?, "
            "?,?,?)"));
    prepStmt->setString(4, (std::string)menu.date);
  } else {
    prepStmt = std::shared_ptr<
        sql::PreparedStatement>(connection->prepareStatement(
        "INSERT INTO Menu (menuName, categoryId,isSurvey) VALUES (?, ?,?)"));
  }
  prepStmt->setString(1, (std::string)menu.menuName);
  prepStmt->setUInt64(2, menu.categoryId);
  prepStmt->setBoolean(3, menu.isSurvey);
  prepStmt->executeUpdate();
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
  return prepStmt->executeUpdate();
}

bool MenuDAO::updateMenu(DTO::Menu menu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "UPDATE Menu SET menuName = ?, categoryId = ?, isSurvey = ?"
          "WHERE menuId = ?"));
  prepStmt->setString(1, (std::string)menu.menuName);
  prepStmt->setUInt64(2, menu.categoryId);
  prepStmt->setBoolean(3, menu.isSurvey);
  prepStmt->setUInt64(4, menu.menuId);
  return prepStmt->executeUpdate();
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
    uint64_t menuTypeId = menuResult->getUInt64("categoryId");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    std::string date = menuResult->getString("date");
    return DTO::Menu{menuId, menuName, menuTypeId, isSurvey, date};
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
    uint64_t menuTypeId = menuResult->getUInt64("categoryId");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    std::string date = menuResult->getString("date");
    menus.emplace_back(menuId, menuName, menuTypeId, isSurvey, date);
  }
  return menus;
}

std::vector<DTO::Menu> MenuDAO::getMenusByType(DTO::Category category) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE categoryId = ?"));
  prepStmt->setUInt64(1, (uint64_t)category);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    std::string date = menuResult->getString("date");
    menus.emplace_back(menuId, menuName, (uint64_t)category, isSurvey, date);
  }
  return menus;
}

uint64_t MenuDAO::getMenuType(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "SELECT categoryId FROM Menu WHERE menuId = ?"));
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  if (menuResult->next()) {
    uint64_t menuTypeId = menuResult->getUInt64("categoryId");
    return menuTypeId;
  }
  throw std::runtime_error("invalid menuId, menuId : " +
                           std::to_string(menuId));
}

std::vector<DTO::Menu> MenuDAO::getMenuByDate(std::string date) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE date = ?"));
  prepStmt->setString(1, date);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    uint64_t menuTypeId = menuResult->getUInt64("categoryId");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    menus.emplace_back(menuId, menuName, menuTypeId, isSurvey, date);
  }
  return menus;
}

std::vector<DTO::Menu> MenuDAO::getMenuByCategory(DTO::Category category) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE categoryId = ?"));
  prepStmt->setUInt64(1, (uint64_t)category);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    std::string date = menuResult->getString("date");
    menus.emplace_back(menuId, menuName, (uint64_t)category, isSurvey, date);
  }
  return menus;
}

std::vector<DTO::Menu> MenuDAO::getSurveyMenu(std::string date) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE date = ? AND "
                                   "isSurvey = 1"));
  prepStmt->setString(1, date);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    uint64_t menuTypeId = menuResult->getUInt64("categoryId");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    menus.emplace_back(menuId, menuName, menuTypeId, isSurvey, date);
  }
  return menus;
}

std::vector<DTO::Menu> MenuDAO::getSurveyMenu(std::string date,
                                              DTO::Category category) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<sql::PreparedStatement>(
      connection->prepareStatement("SELECT * FROM Menu WHERE date = ? AND "
                                   "categoryId = ? AND isSurvey = 1"));
  prepStmt->setString(1, date);
  prepStmt->setUInt64(2, (uint64_t)category);
  std::shared_ptr<sql::ResultSet> menuResult(prepStmt->executeQuery());
  std::vector<DTO::Menu> menus;
  while (menuResult->next()) {
    uint64_t menuId = menuResult->getUInt64("menuId");
    std::string menuName = menuResult->getString("menuName");
    bool isSurvey = menuResult->getBoolean("isSurvey");
    menus.emplace_back(menuId, menuName, (uint64_t)category, isSurvey, date);
  }
  return menus;
}