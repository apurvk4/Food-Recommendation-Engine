#pragma once

#include "DAO/IUserDAO.h"
#include "DbConnection.h"

namespace DAO {

class UserDAO : public IUserDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  UserDAO();
  std::vector<User> getAllUsers() override;
  User getUserById(int id) override;
  bool addUser(User user) override;
  bool updateUser(User user) override;
  bool deleteUser(int id) override;
  bool changePassword(int id, std::string newPassword) override;
  uint64_t getLastNotificationId(int userId) override;
};

}; // namespace DAO