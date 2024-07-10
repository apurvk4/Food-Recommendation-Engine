#include "DAO/DiscardFeedbackAnswerDAO.h"
#include "DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::DiscardFeedbackAnswerDAO;

DiscardFeedbackAnswerDAO::DiscardFeedbackAnswerDAO()
    : dbConnection{DbConnection::getInstance()} {}

bool DiscardFeedbackAnswerDAO::addAnswer(DTO::DiscardFeedbackAnswer answer) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "INSERT INTO DiscardFeedbackAnswer (questionId, userId, answer)"
          " VALUES (?, ?, ?)")};
  preparedStatement->setUInt64(1, answer.questionId);
  preparedStatement->setUInt64(2, answer.userId);
  preparedStatement->setString(3, (std::string)answer.answer);
  return preparedStatement->executeUpdate();
}

bool DiscardFeedbackAnswerDAO::deleteAnswer(uint64_t answerId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "DELETE FROM DiscardFeedbackAnswer WHERE answerId = ?")};
  preparedStatement->setUInt64(1, answerId);
  return preparedStatement->executeUpdate();
}

bool DiscardFeedbackAnswerDAO::updateAnswer(DTO::DiscardFeedbackAnswer answer) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "UPDATE DiscardFeedbackAnswer SET answer = ? WHERE answerId = ?")};
  preparedStatement->setString(1, (std::string)answer.answer);
  preparedStatement->setUInt64(2, answer.answerId);
  return preparedStatement->executeUpdate();
}

DTO::DiscardFeedbackAnswer
DiscardFeedbackAnswerDAO::getAnswerById(uint64_t answerId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM DiscardFeedbackAnswer WHERE answerId = ?")};
  preparedStatement->setUInt64(1, answerId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  DTO::DiscardFeedbackAnswer answer;
  if (resultSet->next()) {
    answer.answerId = resultSet->getUInt64("answerId");
    answer.questionId = resultSet->getUInt64("questionId");
    answer.userId = resultSet->getUInt64("userId");
    answer.answer = resultSet->getString("answer");
  }
  return answer;
}

std::vector<DTO::DiscardFeedbackAnswer>
DiscardFeedbackAnswerDAO::getAllAnswers() {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::Statement> statement{connection->createStatement()};
  std::shared_ptr<sql::ResultSet> resultSet{
      statement->executeQuery("SELECT * FROM DiscardFeedbackAnswer")};
  std::vector<DTO::DiscardFeedbackAnswer> answers;
  while (resultSet->next()) {
    DTO::DiscardFeedbackAnswer answer;
    answer.answerId = resultSet->getUInt64("answerId");
    answer.questionId = resultSet->getUInt64("questionId");
    answer.userId = resultSet->getUInt64("userId");
    answer.answer = resultSet->getString("answer");
    answers.push_back(answer);
  }
  return answers;
}

std::vector<DTO::DiscardFeedbackAnswer>
DiscardFeedbackAnswerDAO::getAnswersByQuestionId(uint64_t questionId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "SELECT * FROM DiscardFeedbackAnswer WHERE questionId = ?")};
  preparedStatement->setUInt64(1, questionId);
  std::shared_ptr<sql::ResultSet> resultSet{preparedStatement->executeQuery()};
  std::vector<DTO::DiscardFeedbackAnswer> answers;
  while (resultSet->next()) {
    DTO::DiscardFeedbackAnswer answer;
    answer.answerId = resultSet->getUInt64("answerId");
    answer.questionId = resultSet->getUInt64("questionId");
    answer.userId = resultSet->getUInt64("userId");
    answer.answer = resultSet->getString("answer");
    answers.push_back(answer);
  }
  return answers;
}
