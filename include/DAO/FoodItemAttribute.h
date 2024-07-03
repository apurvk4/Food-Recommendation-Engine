#pragma once

#include "DbConnection.h"
#include "IFoodItemAttribute.h"
#include <cstdint>

namespace DAO {

class FoodItemAttribute : public IFoodItemAttribute {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FoodItemAttribute();
  bool addFoodItemAttribute(uint64_t foodItemId, uint64_t attributeId) override;
  bool removeFoodItemAttribute(uint64_t foodItemId,
                               uint64_t attributeId) override;
  std::vector<std::pair<uint64_t, std::string>>
  getFoodItemAttributes(uint64_t foodItemId) override;
  std::vector<std::pair<uint64_t, std::string>> getAllAttributes() override;
  bool addFoodItemAttribute(std::vector<uint64_t> attributeIds,
                            uint64_t foodItemId) override;
  std::vector<uint64_t>
  getFoodItemsWithGivenAttributes(uint64_t attributeId) override;
};

}; // namespace DAO