#include "ChefController.h"
#include "ChefService.h"
#include "DAO/RoleDAO.h"
#include "MenuDAO.h"
#include "MenuItemDAO.h"
#include "NotificationDAO.h"
#include "PreferenceDAO.h"
#include "RecommendationService.h"
#include "ReviewDAO.h"
#include "ScheduledMenuDAO.h"
#include <memory>

ChefController::ChefController()
    : chefService(std::make_shared<DAO::UserDAO>(),
                  std::make_shared<DAO::RoleDAO>(),
                  std::make_shared<DAO::NotificationDAO>(),
                  std::make_shared<DAO::ScheduledMenuDAO>(),
                  std::make_shared<DAO::ScheduledMenuDAO>(),
                  std::make_shared<DAO::MenuDAO>(),
                  std::make_shared<DAO::FoodItemDAO>(),
                  std::make_shared<DAO::MenuItemDAO>(),
                  std::make_shared<DAO::ReviewDAO>(),
                  std::make_shared<DAO::PreferenceDAO>()),
      recommendationService(std::make_shared<DAO::ReviewDAO>(),
                            std::make_shared<DAO::FoodItemDAO>()) {}

Array<DTO::FoodItem> ChefController::getRecommendationFoodItems(
    U64 chefUserId, DTO::FoodItemType foodItemType, uint32_t count) {
  // verify the userId is a chef
  if (chefService.getRole(chefUserId).roleId != (U64)3) {
    throw std::invalid_argument("User is not a chef");
  }
  auto recommendedItems =
      recommendationService.getRecommendedFoodItems(foodItemType, count);
  Array<DTO::FoodItem> resultArray;
  for (auto item : recommendedItems) {
    resultArray.push_back(item);
  }
  return resultArray;
}