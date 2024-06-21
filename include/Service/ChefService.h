#pragma once

#include "DTO/Role.h"
#include "FoodItem.h"
#include "IFoodItemDAO.h"
#include "IMenuDAO.h"
#include "IMenuItemDAO.h"
#include "IPreferenceDAO.h"
#include "IReviewDAO.h"
#include "IScheduledMenuDAO.h"
#include "MenuType.h"
#include "Preference.h"
#include "Service/UserService.h"
#include <cstdint>
#include <string>
#include <vector>


namespace Service {

using DAO::IFoodItemDAO;
using DAO::IMenuDAO;
using DAO::IMenuItemDAO;
using DAO::IPreferenceDAO;
using DAO::IReviewDAO;
using DAO::IRoleDAO;
using DAO::IScheduledMenuDAO;
using DAO::IUserDAO;
using DTO::FoodItem;
using DTO::Menu;
using DTO::MenuItem;
using DTO::MenuType;

struct UserResponse {
  uint64_t foodItemName;
  uint32_t numberOfVotes;
  uint32_t numberOfLikes;
  uint32_t numberOfDislikes;
  UserResponse(uint64_t foodItemName, uint32_t numberOfVotes,
               uint32_t numberOfLikes, uint32_t numberOfDislikes)
      : foodItemName(foodItemName), numberOfVotes(numberOfVotes),
        numberOfLikes(numberOfLikes), numberOfDislikes(numberOfDislikes) {}
};

class ChefService : public UserService {
  std::shared_ptr<IScheduledMenuDAO> scheduledDAO;
  std::shared_ptr<IScheduledMenuDAO> scheduledMenuDAO;
  std::shared_ptr<IMenuDAO> menuDAO;
  std::shared_ptr<IFoodItemDAO> foodItemDAO;
  std::shared_ptr<IMenuItemDAO> menuItemDAO;
  std::shared_ptr<IReviewDAO> ReviewDAO;
  std::shared_ptr<IPreferenceDAO> preferenceDAO;

public:
  ChefService(std::shared_ptr<IUserDAO> userDAO,
              std::shared_ptr<IRoleDAO> roleDAO,
              std::shared_ptr<INotificationDAO> notificationDAO,
              std::shared_ptr<IScheduledMenuDAO> scheduledDAO,
              std::shared_ptr<IScheduledMenuDAO> scheduledMenuDAO,
              std::shared_ptr<IMenuDAO> menuDAO,
              std::shared_ptr<IFoodItemDAO> foodItemDAO,
              std::shared_ptr<IMenuItemDAO> menuItemDAO,
              std::shared_ptr<IReviewDAO> ReviewDAO,
              std::shared_ptr<IPreferenceDAO> preferenceDAO);
  bool createSurveyMenu(uint64_t chefUserId, Menu menu,
                        std::vector<MenuItem> foodItems);
  bool setTodaysMenu(uint64_t chefUserId, Menu menu,
                     std::vector<MenuItem> foodItems);
  std::vector<Menu> getAvailableMenus(uint64_t chefUserId, uint64_t menuType);
  std::vector<FoodItem> getFoodItems(uint64_t chefUserId, uint64_t menuId);
  std::pair<Menu, std::vector<FoodItem>>
  getNextRecommendedMenu(uint64_t chefUserId, MenuType menuType);
  std::vector<UserResponse>
  getUserPreferences(uint64_t chefUserId, MenuType menuType, std::string date);
  void getReport(uint64_t chefUserId, std::string startDate,
                 std::string endDate);
};
}; // namespace Service