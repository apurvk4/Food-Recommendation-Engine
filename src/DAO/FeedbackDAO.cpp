#include "DAO/FeedbackDAO.h"
#include "DTO/Feedback.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::FeedbackDAO;
using DTO::Feedback;

FeedbackDAO::FeedbackDAO() : dbConnection{DbConnection::getInstance()} {}

bool FeedbackDAO::addFeedback(Feedback feedback) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addFeedbackStatement(
      connection->prepareStatement("INSERT INTO Feedback (userId, "
                                   "menuId,foodItemId, rating, comment, date) "
                                   "VALUES (?, ?,?, ?, ?, ?)"));
  addFeedbackStatement->setUInt64(1, feedback.userId);
  addFeedbackStatement->setUInt64(2, feedback.menuId);
  addFeedbackStatement->setUInt64(3, feedback.foodItemId);
  addFeedbackStatement->setInt(4, feedback.rating);
  addFeedbackStatement->setString(5, (std::string)feedback.comment);
  addFeedbackStatement->setString(6, (std::string)feedback.date);
  return addFeedbackStatement->execute();
}

Feedback FeedbackDAO::getFeedbackById(uint64_t feedbackId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement(
          "SELECT * FROM Feedback WHERE feedbackId = ?"));
  getFeedbackStatement->setUInt64(1, feedbackId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  if (feedbackResult->next()) {
    feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    uint64_t foodItemId = feedbackResult->getUInt64("foodItemId");
    uint32_t rating = feedbackResult->getUInt("rating");
    SString comment = (std::string)feedbackResult->getString("comment");
    SString date = (std::string)feedbackResult->getString("date");
    return Feedback{feedbackId, userId,  menuId, foodItemId,
                    rating,     comment, date};
  }
  throw std::invalid_argument(
      "Invalid feedbackId, cannot find feedback with given "
      "feedbackId : " +
      std::to_string(feedbackId));
}

std::vector<Feedback> FeedbackDAO::getFeedbacksByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement("SELECT * FROM Feedback WHERE menuId = ?"));
  getFeedbackStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    uint64_t foodItemId = feedbackResult->getUInt64("foodItemId");
    int rating = feedbackResult->getInt("rating");
    SString comment = (std::string)feedbackResult->getString("comment");
    SString date = (std::string)feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, foodItemId, rating,
                           comment, date);
  }
  return feedbacks;
}

std::vector<Feedback> FeedbackDAO::getFeedbacksByUserId(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement("SELECT * FROM Feedback WHERE userId = ?"));
  getFeedbackStatement->setUInt64(1, userId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    uint64_t foodItemId = feedbackResult->getUInt64("foodItemId");
    int rating = feedbackResult->getInt("rating");
    SString comment = (std::string)feedbackResult->getString("comment");
    SString date = (std::string)feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, foodItemId, rating,
                           comment, date);
  }
  return feedbacks;
}

std::vector<Feedback>
FeedbackDAO::getFeedbacksByFoodItemId(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getFeedbackStatement(
      connection->prepareStatement(
          "SELECT * FROM Feedback WHERE foodItemId = ?"));
  getFeedbackStatement->setUInt64(1, foodItemId);
  std::unique_ptr<sql::ResultSet> feedbackResult(
      getFeedbackStatement->executeQuery());
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    foodItemId = feedbackResult->getUInt64("foodItemId");
    int rating = feedbackResult->getInt("rating");
    SString comment = (std::string)feedbackResult->getString("comment");
    SString date = (std::string)feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, foodItemId, rating,
                           comment, date);
  }
  return feedbacks;
}

std::vector<Feedback> FeedbackDAO::getAllFeedbacks() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> feedbackResult(
      statement->executeQuery("SELECT * FROM Feedback"));
  std::vector<Feedback> feedbacks;
  while (feedbackResult->next()) {
    uint64_t feedbackId = feedbackResult->getUInt64("feedbackId");
    uint64_t userId = feedbackResult->getUInt64("userId");
    uint64_t menuId = feedbackResult->getUInt64("menuId");
    uint64_t foodItemId = feedbackResult->getUInt64("foodItemId");
    int rating = feedbackResult->getInt("rating");
    SString comment = (std::string)feedbackResult->getString("comment");
    SString date = (std::string)feedbackResult->getString("date");
    feedbacks.emplace_back(feedbackId, userId, menuId, foodItemId, rating,
                           comment, date);
  }
  return feedbacks;
}