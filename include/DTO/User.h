#pragma once

#include <string>

namespace DTO {

struct User {
  uint64_t userId;
  std::string name;
  std::string password;
  uint64_t roleId;
  uint64_t lastNotificationId;
  User(uint64_t userId, std::string name, std::string password, uint64_t roleId,
       uint64_t lastNotificationId)
      : userId(userId), name(name), password(password), roleId(roleId),
        lastNotificationId(lastNotificationId) {}
};

}; // namespace DTO