#include "DAO/LoginDAO.h"
#include "DbConnection.h"
#include "ILoginDAO.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <exception>

using DAO::ILoginDAO;
using DAO::LoginDAO;

LoginDAO::LoginDAO() : dbConnection(DbConnection::getInstance()) {}

bool LoginDAO::login(DTO::Login login) {
  std::shared_ptr<sql::Connection> connection = dbConnection->getConnection();
  try {
    std::shared_ptr<sql::PreparedStatement> statement(
        connection->prepareStatement("INSERT INTO Login (userId) VALUES (?)"));

    statement->setUInt64(1, login.roleId);
    statement->execute();
    return true;
  } catch (std::exception &e) {
    std::cout << "sql exception : " << e.what() << std::endl;
    return false;
  }
}