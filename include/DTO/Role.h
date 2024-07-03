#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

enum class Role : uint64_t { Admin = 1, Employee = 2, Chef = 3 };

inline std::string RoleToString(Role role) {
  switch (role) {
  case Role::Admin:
    return "Admin";
  case Role::Employee:
    return "Employee";
  case Role::Chef:
    return "Chef";
  default:
    throw std::invalid_argument("Invalid role");
  }
}

inline Role StringToRole(const std::string &role) {
  if (role == "Admin") {
    return Role::Admin;
  } else if (role == "Employee") {
    return Role::Employee;
  } else if (role == "Chef") {
    return Role::Chef;
  } else {
    throw std::invalid_argument("Invalid role");
  }
}

}; // namespace DTO