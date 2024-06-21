#include "DAO/ReviewDAO.h"
#include "DTO/Review.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::ReviewDAO;
using DTO::Review;

ReviewDAO::ReviewDAO() : dbConnection{DbConnection::getInstance()} {}

bool ReviewDAO::addReview(Review Review) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> addReviewStatement(
      connection->prepareStatement("INSERT INTO Review (userId, "
                                   "menuId,foodItemId, rating, comment, date) "
                                   "VALUES (?, ?,?, ?, ?, ?)"));
  addReviewStatement->setUInt64(1, Review.userId);
  addReviewStatement->setUInt64(3, Review.foodItemId);
  addReviewStatement->setInt(4, Review.rating);
  addReviewStatement->setString(5, (std::string)Review.comment);
  addReviewStatement->setString(6, (std::string)Review.date);
  return addReviewStatement->execute();
}

Review ReviewDAO::getReviewById(uint64_t ReviewId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getReviewStatement(
      connection->prepareStatement("SELECT * FROM Review WHERE ReviewId = ?"));
  getReviewStatement->setUInt64(1, ReviewId);
  std::unique_ptr<sql::ResultSet> ReviewResult(
      getReviewStatement->executeQuery());
  if (ReviewResult->next()) {
    ReviewId = ReviewResult->getUInt64("ReviewId");
    uint64_t userId = ReviewResult->getUInt64("userId");
    uint64_t menuId = ReviewResult->getUInt64("menuId");
    uint64_t foodItemId = ReviewResult->getUInt64("foodItemId");
    uint32_t rating = ReviewResult->getUInt("rating");
    SString comment = (std::string)ReviewResult->getString("comment");
    SString date = (std::string)ReviewResult->getString("date");
    return Review{ReviewId, userId, foodItemId, rating, comment, date};
  }
  throw std::invalid_argument("Invalid ReviewId, cannot find Review with given "
                              "ReviewId : " +
                              std::to_string(ReviewId));
}

std::vector<Review> ReviewDAO::getReviewsByMenuId(uint64_t menuId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getReviewStatement(
      connection->prepareStatement("SELECT * FROM Review WHERE menuId = ?"));
  getReviewStatement->setUInt64(1, menuId);
  std::unique_ptr<sql::ResultSet> ReviewResult(
      getReviewStatement->executeQuery());
  std::vector<Review> Reviews;
  while (ReviewResult->next()) {
    uint64_t ReviewId = ReviewResult->getUInt64("ReviewId");
    uint64_t userId = ReviewResult->getUInt64("userId");
    uint64_t menuId = ReviewResult->getUInt64("menuId");
    uint64_t foodItemId = ReviewResult->getUInt64("foodItemId");
    int rating = ReviewResult->getInt("rating");
    SString comment = (std::string)ReviewResult->getString("comment");
    SString date = (std::string)ReviewResult->getString("date");
    Reviews.emplace_back(ReviewId, userId, foodItemId, rating, comment, date);
  }
  return Reviews;
}

std::vector<Review> ReviewDAO::getReviewsByUserId(uint64_t userId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getReviewStatement(
      connection->prepareStatement("SELECT * FROM Review WHERE userId = ?"));
  getReviewStatement->setUInt64(1, userId);
  std::unique_ptr<sql::ResultSet> ReviewResult(
      getReviewStatement->executeQuery());
  std::vector<Review> Reviews;
  while (ReviewResult->next()) {
    uint64_t ReviewId = ReviewResult->getUInt64("ReviewId");
    userId = ReviewResult->getUInt64("userId");
    uint64_t menuId = ReviewResult->getUInt64("menuId");
    uint64_t foodItemId = ReviewResult->getUInt64("foodItemId");
    int rating = ReviewResult->getInt("rating");
    SString comment = (std::string)ReviewResult->getString("comment");
    SString date = (std::string)ReviewResult->getString("date");
    Reviews.emplace_back(ReviewId, userId, foodItemId, rating, comment, date);
  }
  return Reviews;
}

std::vector<Review> ReviewDAO::getReviewsByFoodItemId(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::PreparedStatement> getReviewStatement(
      connection->prepareStatement(
          "SELECT * FROM Review WHERE foodItemId = ?"));
  getReviewStatement->setUInt64(1, foodItemId);
  std::unique_ptr<sql::ResultSet> ReviewResult(
      getReviewStatement->executeQuery());
  std::vector<Review> Reviews;
  while (ReviewResult->next()) {
    uint64_t ReviewId = ReviewResult->getUInt64("ReviewId");
    uint64_t userId = ReviewResult->getUInt64("userId");
    foodItemId = ReviewResult->getUInt64("foodItemId");
    int rating = ReviewResult->getInt("rating");
    SString comment = (std::string)ReviewResult->getString("comment");
    SString date = (std::string)ReviewResult->getString("date");
    Reviews.emplace_back(ReviewId, userId, foodItemId, rating, comment, date);
  }
  return Reviews;
}

std::vector<Review> ReviewDAO::getAllReviews() {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  std::unique_ptr<sql::Statement> statement(connection->createStatement());
  std::unique_ptr<sql::ResultSet> ReviewResult(
      statement->executeQuery("SELECT * FROM Review"));
  std::vector<Review> Reviews;
  while (ReviewResult->next()) {
    uint64_t ReviewId = ReviewResult->getUInt64("ReviewId");
    uint64_t userId = ReviewResult->getUInt64("userId");
    uint64_t menuId = ReviewResult->getUInt64("menuId");
    uint64_t foodItemId = ReviewResult->getUInt64("foodItemId");
    int rating = ReviewResult->getInt("rating");
    SString comment = (std::string)ReviewResult->getString("comment");
    SString date = (std::string)ReviewResult->getString("date");
    Reviews.emplace_back(ReviewId, userId, foodItemId, rating, comment, date);
  }
  return Reviews;
}