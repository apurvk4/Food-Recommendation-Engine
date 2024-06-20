#pragma once

#include "DTO/Login.h"
#include "DTO/User.h"

namespace DAO {
class ILoginDAO {
public:
  virtual bool login(DTO::Login login) = 0;
  virtual ~ILoginDAO() = default;
};
}; // namespace DAO