#pragma once

#include <cstdint>
#include <string>

namespace DTO {

struct Role {
  uint64_t roleId;
  std::string roleName;
  Role(uint64_t roleId, std::string roleName)
      : roleId(roleId), roleName(roleName) {}
};

}; // namespace DTO