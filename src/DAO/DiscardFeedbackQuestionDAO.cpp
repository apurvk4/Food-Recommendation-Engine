#include "DAO/DiscardFeedbackQuestionDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::DiscardFeedbackQuestionDAO;

DiscardFeedbackQuestionDAO::DiscardFeedbackQuestionDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool DiscardFeedbackQuestionDAO::addQuestion(
    DTO::DiscardFeedbackQuestion question) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "INSERT INTO DiscardFeedbackQuestion (foodItemId, question)"
          " VALUES (?, ?)")};
  preparedStatement->setUInt64(1, question.foodItemId);
  preparedStatement->setString(2, (std::string)question.question);
  return preparedStatement->executeUpdate();
}

bool DiscardFeedbackQuestionDAO::deleteQuestion(uint64_t questionId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "DELETE FROM DiscardFeedbackQuestion WHERE questionId = ?")};
  preparedStatement->setUInt64(1, questionId);
  return preparedStatement->executeUpdate();
}

bool DiscardFeedbackQuestionDAO::updateQuestion(
    DTO::DiscardFeedbackQuestion question) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement("UPDATE DiscardFeedbackQuestion SET "
                                   "question = ? WHERE questionId = ?")};
  preparedStatement->setString(1, (std::string)question.question);
  preparedStatement->setUInt64(2, question.questionId);
  return preparedStatement->executeUpdate();
}

DTO::DiscardFeedbackQuestion
DiscardFeedbackQuestionDAO::getQuestionById(uint64_t questionId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM DiscardFeedbackQuestion WHERE questionId = ?")};
  preparedStatement->setUInt64(1, questionId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  DTO::DiscardFeedbackQuestion question;
  if (resultSet->next()) {
    question.questionId = resultSet->getUInt64("questionId");
    question.foodItemId = resultSet->getUInt64("foodItemId");
    question.question = resultSet->getString("question");
  }
  return question;
}

std::vector<DTO::DiscardFeedbackQuestion>
DiscardFeedbackQuestionDAO::getAllQuestions() {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::Statement> statement{connection->createStatement()};
  std::shared_ptr<sql::ResultSet> resultSet{
      statement->executeQuery("SELECT * FROM DiscardFeedbackQuestion")};
  std::vector<DTO::DiscardFeedbackQuestion> questions;
  while (resultSet->next()) {
    DTO::DiscardFeedbackQuestion question;
    question.questionId = resultSet->getUInt64("questionId");
    question.foodItemId = resultSet->getUInt64("foodItemId");
    question.question = resultSet->getString("question");
    questions.push_back(question);
  }
  return questions;
}

std::vector<DTO::DiscardFeedbackQuestion>
DiscardFeedbackQuestionDAO::getQuestionsByFoodItemId(uint64_t foodItemId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM DiscardFeedbackQuestion WHERE foodItemId = ?")};
  preparedStatement->setUInt64(1, foodItemId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::DiscardFeedbackQuestion> questions;
  while (resultSet->next()) {
    DTO::DiscardFeedbackQuestion question;
    question.questionId = resultSet->getUInt64("questionId");
    question.foodItemId = resultSet->getUInt64("foodItemId");
    question.question = resultSet->getString("question");
    questions.push_back(question);
  }
  return questions;
}
