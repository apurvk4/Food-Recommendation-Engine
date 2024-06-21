#pragma once

#include "DTO/User.h"
#include <vector>

namespace DAO {

using DTO::User;

class IUserDAO {
public:
  virtual std::vector<User> getAllUsers() = 0;
  virtual User getUserById(int id) = 0;
  virtual bool addUser(User user) = 0;
  virtual bool updateUser(User user) = 0;
  virtual bool deleteUser(int id) = 0;
  virtual bool changePassword(int id, std::string newPassword) = 0;
  virtual uint64_t getLastNotificationId(int userId) = 0;
  virtual ~IUserDAO() = default;
};

}; // namespace DAO