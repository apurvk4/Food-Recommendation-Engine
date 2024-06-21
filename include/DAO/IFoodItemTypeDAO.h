#pragma once

#include <cstdint>
#include <string>

namespace DAO {

class IFoodItemTypeDAO {
public:
  virtual bool addFoodItemType(std::string foodItemType) = 0;
  virtual bool deleteFoodItemType(uint64_t foodItemTypeId) = 0;
  virtual bool updateFoodItemType(uint64_t foodItemTypeId,
                                  std::string foodItemType) = 0;
  virtual std::string getFoodItemTypeById(uint64_t foodItemTypeId) = 0;
  virtual std::string getFoodItemTypeByName(std::string foodItemType) = 0;
  virtual ~IFoodItemTypeDAO() = default;
};

}; // namespace DAO