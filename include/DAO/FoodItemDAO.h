#pragma once

#include "DAO/IFoodItemDAO.h"
#include "DbConnection.h"
#include <memory>

namespace DAO {

class FoodItemDAO : public IFoodItemDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FoodItemDAO();
  bool addFoodItem(DTO::FoodItem) override;
  bool deleteFoodItem(uint64_t foodItemId) override;
  bool updateFoodItem(DTO::FoodItem) override;
  DTO::FoodItem getFoodItemById(uint64_t foodItemId) override;
  std::vector<DTO::FoodItem> getFoodItemsByPrice(double minPrice,
                                                 double maxPrice) override;
  std::vector<DTO::FoodItem> getAvailableFoodItems() override;
  std::vector<DTO::FoodItem>
  getFoodItemsByType(DTO::FoodItemType foodItemType) override;
  std::vector<DTO::FoodItem> getAllFoodItems() override;
};

}; // namespace DAO