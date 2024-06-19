#pragma once

#include "DAO/IRoleDAO.h"
#include "DbConnection.h"
#include <cstdint>
#include <memory>

namespace DAO {

class RoleDAO : public IRoleDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  RoleDAO();
  virtual bool createRole(const std::string &roleName) override;
  virtual bool updateRole(uint64_t roleId, std::string newroleName) override;
  virtual std::string getRoleName(uint64_t roleId) override;
  virtual uint64_t getRoleId(std::string roleName) override;
  virtual std::vector<DTO::Role> getRoles() override;
};

}; // namespace DAO