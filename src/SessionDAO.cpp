// #include "SessionDAO.h"
// #include <cppconn/prepared_statement.h>
// #include <cppconn/resultset.h>
// #include <cppconn/statement.h>

// SessionDAO::SessionDAO() : dbConnection(DbConnection::getInstance()) {}

// std::pair<bool, Session> SessionDAO::findUserSession(uint64_t userId) {
//   std::shared_ptr<sql::Connection> connection =
//   dbConnection->getConnection(); std::shared_ptr<sql::PreparedStatement>
//   prepStmt(
//       connection->prepareStatement("SELECT * FROM sessions WHERE user_id =
//       ?"));
//   prepStmt->setUInt64(1, userId);
//   std::shared_ptr<sql::ResultSet> sessionResult(prepStmt->executeQuery());
//   if (sessionResult->next()) {
//     std::string sessionId = sessionResult->getString("sessionId");
//     uint64_t userId = sessionResult->getUInt64("userId");
//     std::string createdAt = sessionResult->getString("createdAt");
//     std::string validTill = sessionResult->getString("validTill");
//     return std::make_pair(true,
//                           Session(userId, sessionId, createdAt, validTill));
//   }
//   return std::make_pair(false, Session(0, "", "", ""));
// }

// bool SessionDAO::deleteSession(std::string sessionToken) {
//   std::shared_ptr<sql::Connection> connection =
//   dbConnection->getConnection(); std::shared_ptr<sql::PreparedStatement>
//   prepStmt(
//       connection->prepareStatement("DELETE FROM sessions WHERE sessionId =
//       ?"));
//   prepStmt->setString(1, sessionToken);
//   return prepStmt->execute();
// }

// uint64_t SessionDAO::getUserIdBySessionToken(std::string sessionToken) {
//   std::shared_ptr<sql::Connection> connection =
//   dbConnection->getConnection(); std::shared_ptr<sql::PreparedStatement>
//   prepStmt(connection->prepareStatement(
//       "SELECT * FROM sessions WHERE sessionId = ?"));
//   prepStmt->setString(1, sessionToken);
//   std::shared_ptr<sql::ResultSet> sessionResult(prepStmt->executeQuery());
//   if (sessionResult->next()) {
//     return sessionResult->getUInt64("userId");
//   }
//   return 0;
// }

// bool SessionDAO::isSessionValid(std::string sessionToken) {
//   std::shared_ptr<sql::Connection> connection =
//   dbConnection->getConnection(); std::shared_ptr<sql::PreparedStatement>
//   prepStmt(connection->prepareStatement(
//       "SELECT * FROM sessions WHERE sessionId = ?"));
//   prepStmt->setString(1, sessionToken);
//   std::shared_ptr<sql::ResultSet> sessionResult(prepStmt->executeQuery());
//   return sessionResult->next();
// }

// bool SessionDAO::updateSession(std::string sessionToken) {
//   std::shared_ptr<sql::Connection> connection =
//   dbConnection->getConnection(); std::shared_ptr<sql::PreparedStatement>
//   prepStmt(connection->prepareStatement(
//       "UPDATE sessions SET validTill = ? WHERE sessionId = ?"));
//   prepStmt->setString(1, "NOW()");
//   prepStmt->setString(2, sessionToken);
//   return prepStmt->execute();
// }