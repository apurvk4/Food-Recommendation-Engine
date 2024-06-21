#pragma once

#include "DTO/Preference.h"
#include <cstdint>
#include <string>
#include <vector>

namespace DAO {

class IPreferenceDAO {
public:
  virtual bool addPreference(uint64_t userId, uint64_t foodItemId,
                             uint64_t menuId, bool preference) = 0;
  virtual std::vector<DTO::Preference>
  getPreferencesByUserId(uint64_t userId) = 0;
  virtual std::vector<DTO::Preference>
  getPreferencesByFoodItemId(uint64_t foodItemId) = 0;
  virtual std::vector<DTO::Preference>
  getPreferenceByMenuId(uint64_t menuId) = 0;
  virtual std::vector<DTO::Preference>
  getPreferenceByDate(std::string date, uint64_t foodItemId) = 0;
  virtual ~IPreferenceDAO() = default;
};

}; // namespace DAO