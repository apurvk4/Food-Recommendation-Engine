#pragma once

#include "DTO/Login.h"
#include "DTO/User.h"
#include "DbConnection.h"
#include "ILoginDAO.h"
#include <memory>

namespace DAO {
class LoginDAO : public ILoginDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  LoginDAO();
  bool login(DTO::Login login) override;
};
}; // namespace DAO