#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct User {
  U64 userId;
  SString name;
  SString password;
  U64 roleId;
  U64 lastNotificationId;
  User() = default;
  User(U64 userId, SString name, SString password, U64 roleId,
       U64 lastNotificationId)
      : userId(userId), name(name), password(password), roleId(roleId),
        lastNotificationId(lastNotificationId) {}

  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> nameSerialized = name.serialize();
    std::vector<unsigned char> passwordSerialized = password.serialize();
    std::vector<unsigned char> roleIdSerialized = roleId.serialize();
    std::vector<unsigned char> lastNotificationIdSerialized =
        lastNotificationId.serialize();
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), nameSerialized.begin(),
                      nameSerialized.end());
    serialized.insert(serialized.end(), passwordSerialized.begin(),
                      passwordSerialized.end());
    serialized.insert(serialized.end(), roleIdSerialized.begin(),
                      roleIdSerialized.end());
    serialized.insert(serialized.end(), lastNotificationIdSerialized.begin(),
                      lastNotificationIdSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) {
    uint64_t bytesRead = 0;
    bytesRead += userId.deserialize(
        std::vector<unsigned char>(data.begin(), data.end()));
    bytesRead += name.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += password.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += roleId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += lastNotificationId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() {
    return userId.getSize() + name.getSize() + password.getSize() +
           roleId.getSize() + lastNotificationId.getSize();
  }
};

}; // namespace DTO