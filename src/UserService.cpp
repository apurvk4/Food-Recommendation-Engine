#include "UserService.h"
#include "IUserDAO.h"

UserService::UserService(std::shared_ptr<IUserDAO> userDAO, User activeUser)
    : userDAO{userDAO}, activeUser{activeUser}, roleVerified{false} {}

std::vector<User> UserService::getAllUsers() { return userDAO->getAllUsers(); }

User UserService::getUserById(int id) { return userDAO->getUserById(id); }

bool UserService::addUser(User user) { return userDAO->addUser(user); }

bool UserService::updateUser(User user) { return userDAO->updateUser(user); }

bool UserService::deleteUser(int id) { return userDAO->deleteUser(id); }

bool UserService::changePassword(int id, std::string newPassword) {
  return userDAO->changePassword(id, newPassword);
}

Roles UserService::getCurrentRole() {
  if (!roleVerified) {
    User user = userDAO->getUserById(activeUser.id);
    if (user.roleId == activeUser.roleId) {
      roleVerified = true;
    }
  }
  return static_cast<Roles>(activeUser.roleId);
}