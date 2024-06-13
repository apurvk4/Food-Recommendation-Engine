#pragma once

#include "DbConnection.h"
#include "ISessionDAO.h"
#include "Session.h"
// class SessionDAO : public ISessionDAO {
//   std::shared_ptr<DbConnection> dbConnection;
//   std::pair<bool, Session> findUserSession(uint64_t userId);

// public:
//   SessionDAO();
//   std::pair<bool, std::string> createSession(uint64_t userId) override;
//   bool deleteSession(std::string sessionToken) override;
//   uint64_t getUserIdBySessionToken(std::string sessionToken) override;
//   bool isSessionValid(std::string sessionToken) override;
//   bool updateSession(std::string sessionToken) override;
//   ~SessionDAO() override = default;
// };