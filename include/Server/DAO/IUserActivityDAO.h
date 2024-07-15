#pragma once

#include "IUserActivityDAO.h"
#include "UserActivity.h"

namespace DAO {

using DTO::UserActivity;

class IUserActivityDAO {
public:
  virtual bool addUserActivity(UserActivity userActivity) = 0;
  virtual std::vector<UserActivity> getUserActivity(uint64_t userId) = 0;
  virtual std::vector<UserActivity>
  getUserActivity(uint64_t userId, uint64_t activityTypeId) = 0;
  virtual std::vector<UserActivity>
  getUserActivity(uint64_t userId, uint64_t from, uint64_t to) = 0;
  virtual ~IUserActivityDAO() = default;
};

}; // namespace DAO