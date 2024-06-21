#include "Service/UserService.h"
#include "DAO/IRoleDAO.h"
#include "DAO/IUserDAO.h"
#include "DTO/Role.h"
#include "DTO/User.h"

using DAO::IUserDAO;
using DTO::Role;
using DTO::User;
using Service::UserService;

UserService::UserService(std::shared_ptr<IUserDAO> userDAO,
                         std::shared_ptr<DAO::IRoleDAO> roleDAO,
                         std::shared_ptr<INotificationDAO> notificationDAO)
    : userDAO{userDAO}, roleDAO{roleDAO} {}

std::vector<User> UserService::getAllUsers() { return userDAO->getAllUsers(); }

User UserService::getUserById(uint64_t id) {
  if (userDAO != nullptr) {
    std::cout << "userDAO not null\n";
    return userDAO->getUserById(id);
  }
  std::cout << "userDAO invalid\n";
  throw std::invalid_argument("userDAO invalid");
}

bool UserService::addUser(User user) { return userDAO->addUser(user); }

bool UserService::updateUser(User user) { return userDAO->updateUser(user); }

bool UserService::deleteUser(uint64_t id) { return userDAO->deleteUser(id); }

bool UserService::changePassword(int id, std::string newPassword) {
  return userDAO->changePassword(id, newPassword);
}

Role UserService::getRole(uint64_t id) {
  std::string RoleName = roleDAO->getRoleName(id);
  return Role(id, RoleName);
}

std::vector<DTO::Notification>
UserService::getUnreadNotifications(uint64_t userId) {
  uint64_t lastNotificationId = userDAO->getLastNotificationId(userId);
  return notificationDAO->getAllNotificationsAfter(lastNotificationId);
}