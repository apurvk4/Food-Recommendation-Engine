#include "Service/UserService.h"
#include "ActivityType.h"
#include "DAO/IUserDAO.h"
#include "DTO/Role.h"
#include "DTO/User.h"
#include "IUserActivityDAO.h"
#include "UserActivity.h"
#include <cstdint>

using DAO::IUserDAO;
using DTO::Role;
using DTO::User;
using Service::UserService;

UserService::UserService(
    std::shared_ptr<IUserDAO> userDAO,
    std::shared_ptr<DAO::IUserActivityDAO> userActivityDAO,
    std::shared_ptr<DAO::INotificationDAO> notificationDAO,
    std::shared_ptr<DAO::IUserFoodPreferenceDAO> userFoodPreferenceDAO)
    : userDAO(userDAO), userActivityDAO(userActivityDAO),
      notificationDAO(notificationDAO),
      userFoodPreferenceDAO(userFoodPreferenceDAO) {}

std::vector<User> UserService::getAllUsers() { return userDAO->getAllUsers(); }

User UserService::getUserById(uint64_t id) {
  if (userDAO != nullptr) {
    return userDAO->getUserById(id);
  }
  std::cout << "userDAO invalid\n";
  throw std::invalid_argument("userDAO invalid");
}

bool UserService::addUser(User user) { return userDAO->addUser(user); }

bool UserService::updateUser(User user) { return userDAO->updateUser(user); }

bool UserService::deleteUser(uint64_t id) { return userDAO->deleteUser(id); }

bool UserService::changePassword(uint64_t userId, std::string newPassword,
                                 std::string oldPassword) {
  User user = userDAO->getUserById(userId);
  if (user.password == oldPassword) {
    user.password = newPassword;
    return userDAO->updateUser(user);
  }
  return false;
}

Role UserService::getRole(uint64_t id) {
  auto user = userDAO->getUserById(id);
  return (Role)(uint64_t)user.roleId;
}

bool UserService::addActivity(uint64_t userId, DTO::ActivityType activityType) {
  DTO::UserActivity userActivity(0, userId, (uint64_t)activityType, 0);
  bool result = userActivityDAO->addUserActivity(userActivity);
  return result;
}

std::vector<DTO::UserActivity> UserService::getAllActivity(uint64_t userId) {
  return userActivityDAO->getUserActivity(userId);
}

std::vector<DTO::Notification>
UserService::getUnreadNotifications(uint64_t userId) {
  User user = userDAO->getUserById(userId);
  auto notifications =
      notificationDAO->getAllNotificationsAfter(user.lastNotificationId);
  if (notifications.size() > 0) {
    uint64_t maxNotificationId = notifications[0].notificationId;
    for (auto notification : notifications) {
      if ((uint64_t)notification.notificationId > maxNotificationId) {
        maxNotificationId = notification.notificationId;
      }
    }
    user.lastNotificationId = maxNotificationId;
    userDAO->updateUser(user);
  }
  return notifications;
}

bool UserService::addUserFoodPreference(
    uint64_t userId, const std::vector<uint64_t> &attributeId) {
  for (auto id : attributeId) {
    if (!userFoodPreferenceDAO->addUserFoodPreference(userId, id)) {
      return false;
    }
  }
  return true;
}

bool UserService::deleteUserFoodPreference(
    uint64_t userId, const std::vector<uint64_t> &attributeId) {
  for (auto id : attributeId) {
    if (!userFoodPreferenceDAO->deleteUserFoodPreference(userId, id)) {
      return false;
    }
  }
  return true;
}

std::vector<uint64_t> UserService::getUserFoodPreference(uint64_t userId) {
  return userFoodPreferenceDAO->getUserFoodPreference(userId);
}