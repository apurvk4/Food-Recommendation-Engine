#pragma once

#include "DbConnection.h"
#include "IUserActivityDAO.h"

namespace DAO {

using DTO::UserActivity;

class UserActivityDAO : public IUserActivityDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  UserActivityDAO();
  bool addUserActivity(UserActivity userActivity) override;
  std::vector<UserActivity> getUserActivity(uint64_t userId) override;
  std::vector<UserActivity> getUserActivity(uint64_t userId,
                                            uint64_t activityTypeId) override;
  std::vector<UserActivity> getUserActivity(uint64_t userId, uint64_t from,
                                            uint64_t to) override;
  ~UserActivityDAO() = default;
};

}; // namespace DAO