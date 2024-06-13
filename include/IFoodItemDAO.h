#pragma once

#include "FoodItem.h"
#include <cstdint>
#include <vector>

class IFoodItemDAO {
public:
  virtual bool addFoodItem(FoodItem) = 0;
  virtual bool deleteFoodItem(uint64_t foodItemId) = 0;
  virtual bool updateFoodItem(FoodItem) = 0;
  virtual FoodItem getFoodItemById(uint64_t foodItemId) = 0;
  virtual std::vector<FoodItem> getFoodItemsByPrice(double minPrice,
                                                    double maxPrice) = 0;
  virtual std::vector<FoodItem> getAvailableFoodItems() = 0;
  virtual std::vector<FoodItem> getAllFoodItems() = 0;
  ~IFoodItemDAO() = default;
};