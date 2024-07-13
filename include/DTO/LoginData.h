#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"

struct LoginData : public Serializable {
  U64 userId;
  U64 roleId;
  SString password;
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> roleIdSerialized = roleId.serialize();
    std::vector<unsigned char> passwordSerialized = password.serialize();
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), roleIdSerialized.begin(),
                      roleIdSerialized.end());
    serialized.insert(serialized.end(), passwordSerialized.begin(),
                      passwordSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> userIdData(data.begin(), data.begin() + 8);
    uint64_t bytesRead = userId.deserialize(userIdData);
    std::vector<unsigned char> roleIdData(data.begin() + 8, data.begin() + 16);
    bytesRead += roleId.deserialize(roleIdData);
    std::vector<unsigned char> passwordData(data.begin() + 16, data.end());
    bytesRead += password.deserialize(passwordData);
    return bytesRead;
  }
  size_t getSize() override {
    return userId.getSize() + password.getSize() + roleId.getSize();
  }
};