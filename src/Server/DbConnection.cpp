#include "Server/DbConnection.h"
#include <cppconn/connection.h>
#include <memory>
#include <mysql_driver.h>
#include <stdexcept>

std::shared_ptr<sql::Connection> DbConnection::connection = nullptr;
std::shared_ptr<DbConnection> DbConnection::instance = nullptr;

std::shared_ptr<sql::Connection> DbConnection::getConnection() {
  if (connection != nullptr) {
    return connection;
  }
  std::runtime_error(
      "Connection is not initialized.Need to call InitDbConnection first");
}

void DbConnection::initDbConnection(const std::string &hostName,
                                    const std::string &userName,
                                    const std::string &password,
                                    const std::string &schemaName) {
  sql::mysql::MySQL_Driver *driver;
  driver = sql::mysql::get_mysql_driver_instance();
  connection = std::shared_ptr<sql::Connection>(
      driver->connect(hostName, userName, password));
  connection->setSchema(schemaName);
}

DbConnection::DbConnection() {}

std::shared_ptr<DbConnection> DbConnection::getInstance() {
  if (instance == nullptr) {
    instance = std::shared_ptr<DbConnection>(new DbConnection());
  }
  return instance;
}

DbConnection::~DbConnection() {
  if (connection != nullptr) {
    connection->close();
  }
}