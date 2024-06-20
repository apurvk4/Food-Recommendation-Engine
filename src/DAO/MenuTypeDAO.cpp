#include "DAO/MenuTypeDAO.h"
#include "DTO/MenuType.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::MenuTypeDAO;
using DTO::MenuType;

MenuTypeDAO::MenuTypeDAO() : dbConnection{DbConnection::getInstance()} {}

std::vector<MenuType> MenuTypeDAO::getAllMenuTypes() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> getAllMenuTypesStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> menuTypeResult(
      getAllMenuTypesStatement->executeQuery("SELECT * FROM MenuType"));
  std::vector<MenuType> menuTypes;
  while (menuTypeResult->next()) {
    uint64_t menuTypeId = menuTypeResult->getUInt64("menuTypeId");
    std::string menuType = menuTypeResult->getString("menuType");
    menuTypes.emplace_back(menuTypeId, menuType);
  }
  return menuTypes;
}

MenuType MenuTypeDAO::getMenuTypeById(uint64_t menuTypeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getMenuTypeStatement(
      connection->prepareStatement(
          "SELECT * FROM MenuType WHERE menuTypeId = ?"));
  getMenuTypeStatement->setUInt64(1, menuTypeId);
  std::unique_ptr<sql::ResultSet> menuTypeResult(
      getMenuTypeStatement->executeQuery());
  if (menuTypeResult->next()) {
    menuTypeId = menuTypeResult->getUInt64("menuTypeId");
    std::string menuType = menuTypeResult->getString("menuType");
    return MenuType{menuTypeId, menuType};
  }
  throw std::invalid_argument("Invalid menuTypeId, cannot find menuType with "
                              "given menuTypeId : " +
                              std::to_string(menuTypeId));
}

MenuType MenuTypeDAO::getMenuTypeByName(std::string menuType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getMenuTypeStatement(
      connection->prepareStatement(
          "SELECT * FROM MenuType WHERE menuType = ?"));
  getMenuTypeStatement->setString(1, menuType);
  std::unique_ptr<sql::ResultSet> menuTypeResult(
      getMenuTypeStatement->executeQuery());
  if (menuTypeResult->next()) {
    uint64_t menuTypeId = menuTypeResult->getUInt64("menuTypeId");
    menuType = menuTypeResult->getString("menuType");
    return MenuType{menuTypeId, menuType};
  }
  throw std::invalid_argument("Invalid menuType, cannot find menuType with "
                              "given menuType : " +
                              menuType);
}

void MenuTypeDAO::addMenuType(MenuType menuType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addMenuTypeStatement(
      connection->prepareStatement(
          "INSERT INTO MenuType (menuType) VALUES (?)"));
  addMenuTypeStatement->setString(1, (std::string)menuType.menuType);
  addMenuTypeStatement->execute();
}

void MenuTypeDAO::updateMenuType(MenuType menuType) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateMenuTypeStatement(
      connection->prepareStatement(
          "UPDATE MenuType SET menuType = ? WHERE menuTypeId = ?"));
  updateMenuTypeStatement->setString(1, (std::string)menuType.menuType);
  updateMenuTypeStatement->setUInt64(2, menuType.menuTypeId);
  updateMenuTypeStatement->execute();
}

void MenuTypeDAO::deleteMenuType(uint64_t menuTypeId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteMenuTypeStatement(
      connection->prepareStatement(
          "DELETE FROM MenuType WHERE menuTypeId = ?"));
  deleteMenuTypeStatement->setUInt64(1, menuTypeId);
  deleteMenuTypeStatement->execute();
}
