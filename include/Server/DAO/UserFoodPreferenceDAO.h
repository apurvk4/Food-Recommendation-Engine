#pragma once

#include "IUserFoodPreferenceDAO.h"
#include "Server/DbConnection.h"
#include <cstdint>
#include <vector>

namespace DAO {

class UserFoodPreferenceDAO : public IUserFoodPreferenceDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  UserFoodPreferenceDAO();
  bool addUserFoodPreference(uint64_t userId, uint64_t attributeId) override;
  bool deleteUserFoodPreference(uint64_t userId, uint64_t attributeId) override;
  bool updateUserFoodPreference(uint64_t userId, uint64_t attributeId) override;
  std::vector<uint64_t> getUserFoodPreference(uint64_t userId) override;
  ~UserFoodPreferenceDAO() = default;
};

}; // namespace DAO