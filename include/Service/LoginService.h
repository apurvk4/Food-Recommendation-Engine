#pragma once

#include "DTO/Login.h"
#include "DTO/User.h"
#include "ILoginDAO.h"
#include "IUserDAO.h"
#include <memory>

namespace service {
class LoginService {
  std::shared_ptr<DAO::IUserDAO> userDAO;
  std::shared_ptr<DAO::ILoginDAO> loginDAO;

public:
  LoginService(std::shared_ptr<DAO::IUserDAO> userDAO,
               std::shared_ptr<DAO::ILoginDAO> loginDAO);
  DTO::User login(DTO::Login login);
  ~LoginService() = default;
};
}; // namespace service