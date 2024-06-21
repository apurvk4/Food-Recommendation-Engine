#pragma once

#include "DAO/IRoleDAO.h"
#include "DAO/UserDAO.h"
#include "DTO/Role.h"
#include "DTO/User.h"
#include "INotificationDAO.h"
#include <cppconn/connection.h>
#include <memory>
#include <vector>

namespace Service {

using DAO::INotificationDAO;
using DAO::IRoleDAO;
using DAO::IUserDAO;
using DTO::Role;
using DTO::User;

class UserService {
protected:
  std::shared_ptr<DAO::IUserDAO> userDAO;
  std::shared_ptr<DAO::IRoleDAO> roleDAO;
  std::shared_ptr<INotificationDAO> notificationDAO;

public:
  UserService(std::shared_ptr<IUserDAO> userDAO,
              std::shared_ptr<IRoleDAO> roleDAO,
              std::shared_ptr<INotificationDAO> notificationDAO);
  std::vector<User> getAllUsers();
  User getUserById(uint64_t userId);
  bool addUser(User user);
  bool updateUser(User user);
  bool deleteUser(uint64_t userId);
  bool changePassword(int id, std::string newPassword);
  Role getRole(uint64_t userId);
  std::vector<DTO::Notification> getUnreadNotifications(uint64_t userId);
};

}; // namespace Service
