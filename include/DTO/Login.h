#pragma once

#include "SerializableTypes/SString.h"
#include <sys/types.h>

#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"

#include <cstdint>

namespace DTO {

struct Login : public Serializable {
  U64 loginId;
  U64 userId;
  U64 roleId;
  SString loginTime;
  Login(U64 loginId, U64 userId, U64 roleId, SString loginTime)
      : loginId(loginId), userId(userId), roleId(roleId), loginTime(loginTime) {
  }
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> loginIdSerialized = loginId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> roleIdSerialized = roleId.serialize();
    std::vector<unsigned char> loginTimeSerialized = loginTime.serialize();
    serialized.insert(serialized.end(), loginIdSerialized.begin(),
                      loginIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), roleIdSerialized.begin(),
                      roleIdSerialized.end());
    serialized.insert(serialized.end(), loginTimeSerialized.begin(),
                      loginTimeSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> loginIdData(data.begin(), data.begin() + 8);
    uint64_t bytesRead = loginId.deserialize(loginIdData);
    std::vector<unsigned char> userIdData(data.begin() + 8, data.begin() + 16);
    bytesRead += userId.deserialize(userIdData);
    std::vector<unsigned char> roleIdData(data.begin() + 16, data.begin() + 24);
    bytesRead += roleId.deserialize(roleIdData);
    std::vector<unsigned char> loginTimeData(data.begin() + 24, data.end());
    bytesRead += loginTime.deserialize(loginTimeData);
    return bytesRead;
  }
  size_t getSize() override {
    return loginId.getSize() + userId.getSize() + roleId.getSize() +
           loginTime.getSize();
  }
};

}; // namespace DTO