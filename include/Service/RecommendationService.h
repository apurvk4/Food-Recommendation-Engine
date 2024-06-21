#pragma once

#include "FoodItem.h"
#include "FoodItemDAO.h"
#include "IReviewDAO.h"
#include "Menu.h"
#include <MenuType.h>
#include <memory>

namespace Service {

class RecommendationService {
private:
  std::shared_ptr<DAO::IReviewDAO> reviewDAO;
  std::shared_ptr<DAO::FoodItemDAO> foodItemDAO;

public:
  RecommendationService(std::shared_ptr<DAO::IReviewDAO> ReviewDAO,
                        std::shared_ptr<DAO::FoodItemDAO> foodItemDAO);
  std::vector<DTO::FoodItem> getRecommendedFoodItems(DTO::FoodItemType,
                                                     U32 count);
};

}; // namespace Service