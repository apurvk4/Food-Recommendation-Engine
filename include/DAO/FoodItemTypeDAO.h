#pragma once

#include "DAO/IFoodItemTypeDAO.h"
#include "DbConnection.h"

namespace DAO {

class FoodItemTypeDAO : public IFoodItemTypeDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FoodItemTypeDAO();
  bool addFoodItemType(std::string foodItemType) override;
  bool deleteFoodItemType(uint64_t foodItemTypeId) override;
  bool updateFoodItemType(uint64_t foodItemTypeId,
                          std::string foodItemType) override;
  std::string getFoodItemTypeById(uint64_t foodItemTypeId) override;
  std::string getFoodItemTypeByName(std::string foodItemType) override;
};

}; // namespace DAO