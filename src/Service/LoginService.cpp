#include "Service/LoginService.h"
#include <exception>
#include <iostream>

using service::LoginService;

LoginService::LoginService(std::shared_ptr<DAO::IUserDAO> userDAO,
                           std::shared_ptr<DAO::ILoginDAO> loginDAO)
    : userDAO(userDAO), loginDAO(loginDAO) {}

DTO::User LoginService::login(DTO::Login login) {
  std::cout << "login service\n";
  std::cout << login.loginId << "\n";
  std::cout << login.userId << "\n";
  std::cout << login.roleId << "\n";
  try {
    bool result = loginDAO->login(login);
    if (result) {
      auto user = userDAO->getUserById(login.userId);
      std::cout << "user found\n";
      return user;
    } else {
      std::cout << "login failed\n";
    }
  } catch (std::exception &e) {
    std::cout << "login failed\n";
  }

  throw std::runtime_error("Login failed");
}
