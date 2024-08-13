#include "Server/DAO/AttributeDAO.h"
#include "Attribute.h"
#include "Server/DbConnection.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using DAO::AttributeDAO;
using DTO::Attribute;

AttributeDAO::AttributeDAO() : dbConnection{DbConnection::getInstance()} {}

U64 AttributeDAO::addAttribute(SString attributeName) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "INSERT INTO Attribute (attributeName) VALUES (?)")};
  preparedStatement->setString(1, (std::string)attributeName);
  preparedStatement->executeUpdate();
  // get last inserted id
  std::shared_ptr<sql::Statement> stmt;
  stmt = std::shared_ptr<sql::Statement>(connection->createStatement());
  std::shared_ptr<sql::ResultSet> attributeResult(
      stmt->executeQuery("SELECT LAST_INSERT_ID() as id"));
  if (attributeResult->next()) {
    return attributeResult->getUInt64("id");
  }
  throw std::runtime_error("Failed to get last inserted id");
}

bool AttributeDAO::removeAttribute(U64 attributeId) {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::PreparedStatement> preparedStatement{
      connection->prepareStatement(
          "DELETE FROM Attribute WHERE attributeId = ?")};
  preparedStatement->setUInt64(1, attributeId);
  return preparedStatement->executeUpdate();
}

std::vector<Attribute> AttributeDAO::getAllAttributes() {
  std::shared_ptr<sql::Connection> connection{dbConnection->getConnection()};
  std::shared_ptr<sql::Statement> stmt;
  stmt = std::shared_ptr<sql::Statement>(connection->createStatement());
  std::shared_ptr<sql::ResultSet> attributeResult(
      stmt->executeQuery("SELECT * FROM Attribute"));
  std::vector<Attribute> attributes;
  while (attributeResult->next()) {
    uint64_t id = attributeResult->getUInt64("attributeId");
    std::string name = attributeResult->getString("attributeName");
    attributes.emplace_back(id, name);
  }
  return attributes;
}