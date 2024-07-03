#include "DAO/FeedbackDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::FeedbackDAO;

FeedbackDAO::FeedbackDAO() : dbConnection{DbConnection::getInstance()} {}

bool FeedbackDAO::addFeedback(DTO::Feedback feedback) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "INSERT INTO Feedback (userId, foodItemId, preference)"
          " VALUES (?, ?, ?)")};
  preparedStatement->setUInt64(1, feedback.userId);
  preparedStatement->setUInt64(2, feedback.foodItemId);
  preparedStatement->setBoolean(3, feedback.preference);
  return preparedStatement->executeUpdate();
}

std::vector<DTO::Feedback> FeedbackDAO::getFeedbackByUserId(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement("SELECT * FROM Feedback WHERE userId = ?")};
  preparedStatement->setUInt64(1, userId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::Feedback> feedbacks;
  while (resultSet->next()) {
    DTO::Feedback feedback;
    feedback.feedbackId = resultSet->getUInt64("feedbackId");
    feedback.userId = resultSet->getUInt64("userId");
    feedback.foodItemId = resultSet->getUInt64("foodItemId");
    feedback.preference = resultSet->getBoolean("preference");
    feedback.date = resultSet->getString("date");
    feedbacks.push_back(feedback);
  }
  return feedbacks;
}

std::vector<DTO::Feedback> FeedbackDAO::getFeedbackByUserId(uint64_t userId,
                                                            std::string date) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM Feedback WHERE userId = ? AND date = ?")};
  preparedStatement->setUInt64(1, userId);
  preparedStatement->setString(2, date);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::Feedback> feedbacks;
  while (resultSet->next()) {
    DTO::Feedback feedback;
    feedback.feedbackId = resultSet->getUInt64("feedbackId");
    feedback.userId = resultSet->getUInt64("userId");
    feedback.foodItemId = resultSet->getUInt64("foodItemId");
    feedback.preference = resultSet->getBoolean("preference");
    feedback.date = resultSet->getString("date");
    feedbacks.push_back(feedback);
  }
  return feedbacks;
}

std::vector<DTO::Feedback>
FeedbackDAO::getFeedbackByFoodItemId(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM Feedback WHERE foodItemId = ?")};
  preparedStatement->setUInt64(1, foodItemId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::Feedback> feedbacks;
  while (resultSet->next()) {
    DTO::Feedback feedback;
    feedback.feedbackId = resultSet->getUInt64("feedbackId");
    feedback.userId = resultSet->getUInt64("userId");
    feedback.foodItemId = resultSet->getUInt64("foodItemId");
    feedback.preference = resultSet->getBoolean("preference");
    feedback.date = resultSet->getString("date");
    feedbacks.push_back(feedback);
  }
  return feedbacks;
}

std::vector<DTO::Feedback>
FeedbackDAO::getFeedbackByFoodItemId(uint64_t foodItemId, std::string date) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM Feedback WHERE foodItemId = ? AND date = ?")};
  preparedStatement->setUInt64(1, foodItemId);
  preparedStatement->setString(2, date);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::Feedback> feedbacks;
  while (resultSet->next()) {
    DTO::Feedback feedback;
    feedback.feedbackId = resultSet->getUInt64("feedbackId");
    feedback.userId = resultSet->getUInt64("userId");
    feedback.foodItemId = resultSet->getUInt64("foodItemId");
    feedback.preference = resultSet->getBoolean("preference");
    feedback.date = resultSet->getString("date");
    feedbacks.push_back(feedback);
  }
  return feedbacks;
}

bool FeedbackDAO::deleteFeedback(uint64_t feedbackId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "DELETE FROM Feedback WHERE feedbackId = ?")};
  preparedStatement->setUInt64(1, feedbackId);
  return preparedStatement->executeUpdate();
}

bool FeedbackDAO::updateFeedback(DTO::Feedback feedback) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "UPDATE Feedback SET userId = ?, foodItemId = ?, preference = ?, "
          "date = ? WHERE feedbackId = ?")};
  preparedStatement->setUInt64(1, feedback.userId);
  preparedStatement->setUInt64(2, feedback.foodItemId);
  preparedStatement->setBoolean(3, feedback.preference);
  preparedStatement->setString(4, (std::string)feedback.date);
  preparedStatement->setUInt64(5, feedback.feedbackId);
  return preparedStatement->executeUpdate();
}