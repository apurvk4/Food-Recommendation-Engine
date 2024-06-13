#pragma once

#include "Roles.h"
#include "User.h"
#include "UserDAO.h"
#include <cppconn/connection.h>
#include <memory>
#include <vector>

class IUserDAO;

class UserService {
  std::shared_ptr<IUserDAO> userDAO;
  User activeUser;
  bool roleVerified;

public:
  UserService(std::shared_ptr<IUserDAO> userDAO, User activeUser);
  std::vector<User> getAllUsers();
  User getUserById(int id);
  bool addUser(User user);
  bool updateUser(User user);
  bool deleteUser(int id);
  bool changePassword(int id, std::string newPassword);
  Roles getCurrentRole();
};
