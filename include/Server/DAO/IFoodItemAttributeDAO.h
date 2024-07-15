#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace DAO {

class IFoodItemAttributeDAO {
public:
  virtual bool addFoodItemAttribute(uint64_t foodItemId,
                                    uint64_t attributeId) = 0;
  virtual bool removeFoodItemAttribute(uint64_t foodItemId,
                                       uint64_t attributeId) = 0;
  virtual std::vector<std::pair<uint64_t, std::string>>
  getFoodItemAttributes(uint64_t foodItemId) = 0;
  virtual std::vector<std::pair<uint64_t, std::string>> getAllAttributes() = 0;
  virtual bool addFoodItemAttribute(std::vector<uint64_t> attributeIds,
                                    uint64_t foodItemId) = 0;
  virtual std::vector<uint64_t>
  getFoodItemsWithGivenAttributes(uint64_t attributeId) = 0;
  virtual ~IFoodItemAttributeDAO() = default;
};

}; // namespace DAO