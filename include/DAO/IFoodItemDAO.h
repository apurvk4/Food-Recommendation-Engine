#pragma once

#include "DTO/FoodItem.h"
#include <cstdint>
#include <vector>

namespace DAO {

class IFoodItemDAO {
public:
  virtual bool addFoodItem(DTO::FoodItem) = 0;
  virtual bool deleteFoodItem(uint64_t foodItemId) = 0;
  virtual bool updateFoodItem(DTO::FoodItem) = 0;
  virtual DTO::FoodItem getFoodItemById(uint64_t foodItemId) = 0;
  virtual std::vector<DTO::FoodItem> getFoodItemsByPrice(double minPrice,
                                                         double maxPrice) = 0;
  virtual std::vector<DTO::FoodItem> getAvailableFoodItems() = 0;
  virtual std::vector<DTO::FoodItem>
  getFoodItemsByType(DTO::FoodItemType foodItemType) = 0;
  virtual std::vector<DTO::FoodItem> getAllFoodItems() = 0;
  ~IFoodItemDAO() = default;
};

}; // namespace DAO