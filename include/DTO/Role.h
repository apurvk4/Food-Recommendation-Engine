#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct Role : public Serializable {
  U64 roleId;
  SString roleName;
  Role(U64 roleId, SString roleName) : roleId(roleId), roleName(roleName) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> roleIdSerialized = roleId.serialize();
    std::vector<unsigned char> roleNameSerialized = roleName.serialize();
    serialized.insert(serialized.end(), roleIdSerialized.begin(),
                      roleIdSerialized.end());
    serialized.insert(serialized.end(), roleNameSerialized.begin(),
                      roleNameSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> roleIdData(data.begin(), data.begin() + 8);
    roleId.deserialize(roleIdData);
    std::vector<unsigned char> roleNameData(data.begin() + 8,
                                            data.begin() + 40);
    roleName.deserialize(roleNameData);
    return 40;
  }
  size_t getSize() override { return 40; }
};

}; // namespace DTO