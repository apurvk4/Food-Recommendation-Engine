#include "FeedbackDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

FeedbackDAO::FeedbackDAO() : dbConnection{DbConnection::getInstance()} {}

bool FeedbackDAO::addFeedback(Feedback feedback) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addFeedbackStatement(
      connection->prepareStatement("INSERT INTO Feedbacks (userId, menuId, "
                                   "rating, comment) VALUES (?, ?, ?, ?)"));
  addFeedbackStatement->setUInt64(1, feedback.userId);
  addFeedbackStatement->setUInt64(2, feedback.menuId);
  addFeedbackStatement->setInt(3, feedback.rating);
  addFeedbackStatement->setString(4, feedback.comment);
  return addFeedbackStatement->execute();
}

bool FeedbackDAO::deleteFeedback(uint64_t feedbackId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> deleteFeedbackStatement(
      connection->prepareStatement(
          "DELETE FROM Feedbacks WHERE feedbackId = ?"));
  deleteFeedbackStatement->setUInt64(1, feedbackId);
  return deleteFeedbackStatement->execute();
}

bool FeedbackDAO::updateFeedback(Feedback feedback) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> updateFeedbackStatement(
      connection->prepareStatement(
          "UPDATE Feedbacks SET userId = ?, menuId = ?, rating = ?, "
          "comment = ? WHERE feedbackId = ?"));
  updateFeedbackStatement->setUInt64(1, feedback.userId);
  updateFeedbackStatement->setUInt64(2, feedback.menuId);
  updateFeedbackStatement->setInt(3, feedback.rating);
  updateFeedbackStatement->setString(4, feedback.comment);
  updateFeedbackStatement->setUInt64(5, feedback.feedbackId);
  return updateFeedbackStatement->execute();
}

Feedback FeedbackDAO::getFeedbackById(uint64_t feedbackId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement(
          "SELECT * FROM Feedbacks WHERE feedbackId = ?"));
  getFeedbackStatement->setUInt64(1, feedbackId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  if (feedbackResult->next()) {
    uint64_t userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    int rating = feedbackResult->getInt("rating");
    std::string comment = feedbackResult->getString("comment");
    std::string date = feedbackResult->getString("date");
    return Feedback(feedbackId, userId, menuId, comment, rating, date);
  }
  throw std::invalid_argument("No Feedback with Id : " +
                              std::to_string(feedbackId));
}

std::vector<Feedback> FeedbackDAO::getFeedbacksByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement("SELECT * FROM Feedbacks WHERE menuId = ?"));
  getFeedbackStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t userId = feedbackResult->getUInt64("userId");
    int rating = feedbackResult->getInt("rating");
    std::string comment = feedbackResult->getString("comment");
    std::string date = feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, comment, rating, date);
  }
  return feedbacks;
}

/**
 * @brief get list of feedbacks given by a user with input userId
 *
 * @param userId
 * @return std::vector<Feedback>
 */
std::vector<Feedback> FeedbackDAO::getFeedbacksByUserId(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement("SELECT * FROM Feedbacks WHERE userId = ?"));
  getFeedbackStatement->setUInt64(1, userId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    int rating = feedbackResult->getInt("rating");
    std::string comment = feedbackResult->getString("comment");
    std::string date = feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, comment, rating, date);
  }
  return feedbacks;
}

std::vector<Feedback> FeedbackDAO::getAllFeedbacks() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> feedbacksResult(
      statement->executeQuery("SELECT * FROM Feedbacks"));
  std::vector<Feedback> feedbacks;
  while (feedbacksResult->next()) {
    uint64_t feedbackId = feedbacksResult->getUInt64("feedbackId");
    uint64_t userId = feedbacksResult->getUInt64("userId");
    uint64_t menuId = feedbacksResult->getUInt64("menuId");
    int rating = feedbacksResult->getInt("rating");
    std::string comment = feedbacksResult->getString("comment");
    std::string date = feedbacksResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, comment, rating, date);
  }
  return feedbacks;
}