#pragma once

#include "ActivityType.h"
#include "DAO/IUserActivityDAO.h"
#include "DAO/UserDAO.h"
#include "DTO/Role.h"
#include "DTO/User.h"
#include "INotificationDAO.h"
#include "IUserFoodPreferenceDAO.h"
#include "Notification.h"
#include "UserActivity.h"
#include <cppconn/connection.h>
#include <memory>
#include <vector>

namespace Service {
using DAO::INotificationDAO;
using DAO::IUserActivityDAO;
using DAO::IUserDAO;
using DAO::IUserFoodPreferenceDAO;
using DTO::ActivityType;
using DTO::Notification;
using DTO::Role;
using DTO::User;

class UserService {
  std::shared_ptr<IUserDAO> userDAO;
  std::shared_ptr<IUserActivityDAO> userActivityDAO;
  std::shared_ptr<INotificationDAO> notificationDAO;
  std::shared_ptr<IUserFoodPreferenceDAO> userFoodPreferenceDAO;

public:
  UserService(std::shared_ptr<IUserDAO> userDAO,
              std::shared_ptr<IUserActivityDAO> userActivityDAO,
              std::shared_ptr<INotificationDAO> notificationDAO,
              std::shared_ptr<IUserFoodPreferenceDAO> userFoodPreferenceDAO);

  std::vector<User> getAllUsers();
  User getUserById(uint64_t userId);
  bool addUser(User user);
  bool updateUser(User user);
  bool deleteUser(uint64_t userId);
  bool changePassword(uint64_t userId, std::string newPassword,
                      std::string oldPassword);
  Role getRole(uint64_t userId);
  bool addActivity(uint64_t userId, ActivityType activityType);
  std::vector<DTO::UserActivity> getAllActivity(uint64_t userId);
  std::vector<Notification> getUnreadNotifications(uint64_t userId);
  bool addUserFoodPreference(uint64_t userId,
                             const std::vector<uint64_t> &attributeId);
  bool deleteUserFoodPreference(uint64_t userId,
                                const std::vector<uint64_t> &attributeId);
  std::vector<uint64_t> getUserFoodPreference(uint64_t userId);
  ~UserService() = default;
};

}; // namespace Service
