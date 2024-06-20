#include "DAO/ScheduledMenuDAO.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::ScheduledMenuDAO;
using DTO::ScheduledMenu;

ScheduledMenuDAO::ScheduledMenuDAO()
    : dbConnection{DbConnection::getInstance()} {}

std::vector<ScheduledMenu> ScheduledMenuDAO::getAllScheduledMenus() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> getAllScheduledMenusStatement(
      connection->createStatement());
  std::unique_ptr<sql::ResultSet> scheduledMenuResult(
      getAllScheduledMenusStatement->executeQuery(
          "SELECT * FROM scheduledMenu"));
  std::vector<ScheduledMenu> scheduledMenus;
  while (scheduledMenuResult->next()) {
    uint64_t scheduledMenuId =
        scheduledMenuResult->getUInt64("scheduledMenuId");
    std::string date = scheduledMenuResult->getString("date");
    uint64_t menuId = scheduledMenuResult->getUInt64("menuId");
    bool isSurveyMenu = scheduledMenuResult->getBoolean("isSurveyMenu");
    scheduledMenus.emplace_back(scheduledMenuId, menuId, date, isSurveyMenu);
  }
  return scheduledMenus;
}

ScheduledMenu ScheduledMenuDAO::getScheduledMenu(uint64_t scheduledMenuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getScheduledMenuStatement(
      connection->prepareStatement(
          "SELECT * FROM scheduledMenu WHERE scheduledMenuId = ?"));
  getScheduledMenuStatement->setUInt64(1, scheduledMenuId);
  std::unique_ptr<sql::ResultSet> scheduledMenuResult(
      getScheduledMenuStatement->executeQuery());
  if (scheduledMenuResult->next()) {
    scheduledMenuId = scheduledMenuResult->getUInt64("scheduledMenuId");
    std::string date = scheduledMenuResult->getString("date");
    uint64_t menuId = scheduledMenuResult->getUInt64("menuId");
    bool isSurveyMenu = scheduledMenuResult->getBoolean("isSurveyMenu");
    return ScheduledMenu{scheduledMenuId, menuId, date, isSurveyMenu};
  }
  throw std::invalid_argument("Invalid scheduledMenuId, cannot find "
                              "scheduledMenu with given scheduledMenuId : " +
                              std::to_string(scheduledMenuId));
}

bool ScheduledMenuDAO::addScheduledMenu(DTO::ScheduledMenu scheduledMenu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt = std::shared_ptr<
      sql::PreparedStatement>(connection->prepareStatement(
      "INSERT INTO scheduledMenu (date, menuId,isSurveyMenu) VALUES (?, ?)"));
  prepStmt->setString(1, (std::string)scheduledMenu.date);
  prepStmt->setUInt64(2, scheduledMenu.menuId);
  prepStmt->setBoolean(3, scheduledMenu.isSurveyMenu);
  return prepStmt->execute();
}

bool ScheduledMenuDAO::deleteScheduledMenu(uint64_t scheduledMenuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "DELETE FROM scheduledMenu WHERE scheduledMenuId = ?"));
  prepStmt->setUInt64(1, scheduledMenuId);
  return prepStmt->execute();
}

bool ScheduledMenuDAO::updateScheduledMenu(DTO::ScheduledMenu scheduledMenu) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt;
  prepStmt =
      std::shared_ptr<sql::PreparedStatement>(connection->prepareStatement(
          "UPDATE scheduledMenu SET date = ?, menuId = ? , isSurveyMenu = ?"
          "WHERE scheduledMenuId = ?"));
  prepStmt->setString(1, (std::string)scheduledMenu.date);
  prepStmt->setUInt64(2, scheduledMenu.menuId);
  prepStmt->setBoolean(3, scheduledMenu.isSurveyMenu);
  prepStmt->setUInt64(4, scheduledMenu.scheduledMenuId);
  return prepStmt->execute();
}

std::vector<ScheduledMenu>
ScheduledMenuDAO::getScheduledMenuByDate(std::string date) {
  date = date.substr(0, 10);
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getScheduledMenuStatement(
      connection->prepareStatement(
          "SELECT * FROM scheduledMenu WHERE date = ?"));
  getScheduledMenuStatement->setString(1, date);
  std::unique_ptr<sql::ResultSet> scheduledMenuResult(
      getScheduledMenuStatement->executeQuery());
  std::vector<ScheduledMenu> result;
  if (scheduledMenuResult->next()) {
    uint64_t scheduledMenuId =
        scheduledMenuResult->getUInt64("scheduledMenuId");
    uint64_t menuId = scheduledMenuResult->getUInt64("menuId");
    bool isSurveyMenu = scheduledMenuResult->getBoolean("isSurveyMenu");
    result.emplace_back(scheduledMenuId, menuId, date, isSurveyMenu);
  }
  return result;
}