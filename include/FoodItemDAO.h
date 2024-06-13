#pragma once

#include "DbConnection.h"
#include "IFoodItemDAO.h"
#include <memory>

class FoodItemDAO : public IFoodItemDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FoodItemDAO();
  bool addFoodItem(FoodItem) override;
  bool deleteFoodItem(uint64_t foodItemId) override;
  bool updateFoodItem(FoodItem) override;
  FoodItem getFoodItemById(uint64_t foodItemId) override;
  std::vector<FoodItem> getFoodItemsByPrice(double minPrice,
                                            double maxPrice) override;
  std::vector<FoodItem> getAvailableFoodItems() override;
  std::vector<FoodItem> getAllFoodItems() override;
};