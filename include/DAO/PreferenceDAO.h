#pragma once

#include "DAO/IPreferenceDAO.h"
#include "DbConnection.h"
#include "Preference.h"
namespace DAO {

class PreferenceDAO : public IPreferenceDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  PreferenceDAO();
  bool addPreference(uint64_t userId, uint64_t foodItemTypeId, uint64_t menuId,
                     bool preference) override;
  std::vector<DTO::Preference> getPreferencesByUserId(uint64_t userId) override;
  std::vector<DTO::Preference>
  getPreferencesByFoodItemId(uint64_t foodItemId) override;
  std::vector<DTO::Preference> getPreferenceByMenuId(uint64_t menuId) override;
  std::vector<DTO::Preference>
  getPreferenceByDate(std::string date, uint64_t foodItemId) override;
};

}; // namespace DAO