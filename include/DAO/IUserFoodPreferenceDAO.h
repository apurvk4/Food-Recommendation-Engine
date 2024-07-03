#pragma once

#include <cstdint>
#include <vector>

namespace DAO {

class IUserFoodPreferenceDAO {
public:
  virtual bool addUserFoodPreference(uint64_t userId, uint64_t attributeId) = 0;
  virtual bool deleteUserFoodPreference(uint64_t userId,
                                        uint64_t attributeId) = 0;
  virtual bool updateUserFoodPreference(uint64_t userId,
                                        uint64_t attributeId) = 0;
  virtual std::vector<uint64_t> getUserFoodPreference(uint64_t userId) = 0;
  virtual ~IUserFoodPreferenceDAO() = default;
};

}; // namespace DAO