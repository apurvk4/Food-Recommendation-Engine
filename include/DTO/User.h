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
    std::vector<unsigned char> userIdData(data.begin(), data.begin() + 8);
    uint64_t bytesRead = userId.deserialize(userIdData);
    std::vector<unsigned char> nameData(data.begin() + bytesRead,
                                        data.begin() + bytesRead + 32);
    bytesRead += name.deserialize(nameData);
    std::vector<unsigned char> passwordData(data.begin() + bytesRead,
                                            data.begin() + bytesRead + 32);
    bytesRead += password.deserialize(passwordData);
    std::vector<unsigned char> roleIdData(data.begin() + bytesRead,
                                          data.begin() + bytesRead + 8);
    bytesRead += roleId.deserialize(roleIdData);
    std::vector<unsigned char> lastNotificationIdData(
        data.begin() + bytesRead, data.begin() + bytesRead + 8);
    bytesRead += lastNotificationId.deserialize(lastNotificationIdData);
    return bytesRead;
  }
  size_t getSize() { return 88; }
};

}; // namespace DTO