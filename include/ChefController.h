#pragma once

#include "ChefService.h"
#include "FoodItem.h"
#include "FoodItemType.h"
#include "RecommendationService.h"
#include "SerializableTypes/Array.h"
#include <memory>
#include <string>
#include <vector>

class ChefController {
  Service::RecommendationService recommendationService;
  Service::ChefService chefService;

public:
  ChefController();
  Array<DTO::FoodItem>
  getRecommendationFoodItems(U64 chefUserId, DTO::FoodItemType foodItemType,
                             uint32_t count);
  ~ChefController() = default;
};