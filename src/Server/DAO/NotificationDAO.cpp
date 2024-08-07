#include "Server/DAO/NotificationDAO.h"
#include "DTO/Notification.h"
#include "Server/DbConnection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdint>

using DAO::NotificationDAO;
using DTO::Notification;

NotificationDAO::NotificationDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool NotificationDAO::addNotification(Notification notification) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
      "INSERT INTO Notification (message) VALUES (?)"));
  prepStmt->setString(1, (std::string)notification.message);
  return prepStmt->executeUpdate();
}

bool NotificationDAO::deleteNotification(uint64_t notificationId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
      "DELETE FROM Notification WHERE notificationId = ?"));
  prepStmt->setUInt64(1, notificationId);
  return prepStmt->executeUpdate();
}

bool NotificationDAO::updateNotification(Notification notification) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
      "UPDATE Notification SET message = ? WHERE notificationId = ?"));
  prepStmt->setString(1, (std::string)notification.message);
  prepStmt->setUInt64(2, notification.notificationId);
  return prepStmt->executeUpdate();
}

Notification NotificationDAO::getNotification(uint64_t notificationId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
      "SELECT * FROM Notification WHERE notificationId = ?"));
  prepStmt->setUInt64(1, notificationId);
  std::shared_ptr<sql::ResultSet> notificationResult(prepStmt->executeQuery());
  if (notificationResult->next()) {
    uint64_t notificationId = notificationResult->getUInt64("notificationId");
    std::string message = notificationResult->getString("message");
    std::string date = notificationResult->getString("date");
    return Notification(notificationId, message, date);
  }
  throw std::invalid_argument("No Notification with Id : " +
                              std::to_string(notificationId));
}

std::vector<Notification>
NotificationDAO::getAllNotificationsAfter(uint64_t notificationId) {
  std::vector<Notification> notifications;
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::shared_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
      "SELECT * FROM Notification WHERE notificationId > ?"));
  prepStmt->setUInt64(1, notificationId);
  std::shared_ptr<sql::ResultSet> notificationResult(prepStmt->executeQuery());
  while (notificationResult->next()) {
    uint64_t notificationId = notificationResult->getUInt64("notificationId");
    std::string message = notificationResult->getString("message");
    std::string date = notificationResult->getString("date");
    notifications.emplace_back(notificationId, message, date);
  }
  return notifications;
}