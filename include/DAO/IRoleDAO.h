#pragma once

#include "DTO/Role.h"
#include <string>
#include <vector>

namespace DAO {

using DTO::Role;

class IRoleDAO {
public:
  virtual ~IRoleDAO() = default;
  virtual bool createRole(const std::string &roleName) = 0;
  virtual bool updateRole(uint64_t roleId, std::string newroleName) = 0;
  virtual std::string getRoleName(uint64_t roleId) = 0;
  virtual uint64_t getRoleId(std::string roleName) = 0;
  virtual std::vector<Role> getRoles() = 0;
};
}; // namespace DAO