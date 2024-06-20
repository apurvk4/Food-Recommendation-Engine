#include "Service/ChefService.h"
#include "FoodItem.h"
#include "MenuType.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>

using DTO::FoodItem;
using Service::ChefService;
using Service::UserResponse;

ChefService::ChefService(std::shared_ptr<IUserDAO> userDAO,
                         std::shared_ptr<IRoleDAO> roleDAO,
                         std::shared_ptr<INotificationDAO> notificationDAO,
                         std::shared_ptr<IScheduledMenuDAO> scheduledDAO,
                         std::shared_ptr<IScheduledMenuDAO> scheduledMenuDAO,
                         std::shared_ptr<IMenuDAO> menuDAO,
                         std::shared_ptr<IFoodItemDAO> foodItemDAO,
                         std::shared_ptr<IMenuItemDAO> menuItemDAO,
                         std::shared_ptr<IFeedbackDAO> feedbackDAO,
                         std::shared_ptr<IPreferenceDAO> preferenceDAO)
    : UserService(userDAO, roleDAO, notificationDAO),
      scheduledMenuDAO(scheduledMenuDAO), menuDAO(menuDAO),
      foodItemDAO(foodItemDAO), menuItemDAO(menuItemDAO),
      feedbackDAO(feedbackDAO), preferenceDAO(preferenceDAO) {}

bool ChefService::createSurveyMenu(uint64_t chefUserId, Menu menu,
                                   std::vector<MenuItem> menuItems) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  auto menuId = menuDAO->addMenu(menu);
  for (auto &menuItem : menuItems) {
    menuItemDAO->addMenuItem(menuItem.foodItemId, menuId, menuItem.quantity);
  }
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);

  // Increment the day by 1 to get tomorrow's date
  now_tm.tm_mday += 1;

  // Normalize the time structure (this handles month/year overflow)
  std::mktime(&now_tm);

  // Set time to 12:00 AM
  now_tm.tm_hour = 0;
  now_tm.tm_min = 0;
  now_tm.tm_sec = 0;

  // Format the time to a string
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  auto date = oss.str();

  DTO::ScheduledMenu scheduledMenu{0, menuId, date, true};
  return true;
}

bool ChefService::setTodaysMenu(uint64_t chefUserId, Menu menu,
                                std::vector<MenuItem> menuItems) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  // check if this menu is already present
  uint64_t menuId = 0;
  try {
    menu = menuDAO->getMenu(menu.menuId);
    menuId = menu.menuId;
  } catch (std::runtime_error &e) {
  }
  if (menuId == 0) {
    menuId = menuDAO->addMenu(menu);
    for (auto &menuItem : menuItems) {
      menuItemDAO->addMenuItem(menuItem.foodItemId, menuId, menuItem.quantity);
    }
  }
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);

  // Format the time to a string
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  auto date = oss.str();

  DTO::ScheduledMenu scheduledMenu{0, menuId, date, true};
  return scheduledMenuDAO->addScheduledMenu(scheduledMenu);
}

std::vector<DTO::Menu> ChefService::getAvailableMenus(uint64_t chefUserId,
                                                      uint64_t menuType) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  return menuDAO->getMenusByType(menuType);
}

std::vector<DTO::FoodItem> ChefService::getFoodItems(uint64_t chefUserId,
                                                     uint64_t menuId) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  auto menuItems = menuItemDAO->getMenuItemByMenuId(menuId);
  std::vector<DTO::FoodItem> foodItems;
  for (auto &menuItem : menuItems) {
    foodItems.push_back(foodItemDAO->getFoodItemById(menuItem.foodItemId));
  }
  return foodItems;
}

std::pair<DTO::Menu, std::vector<DTO::FoodItem>>
ChefService::getNextRecommendedMenu(uint64_t chefUserId,
                                    DTO::MenuType menuType) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  // auto menu = menuDAO->getMenusByType(menuType.menuTypeId);
  // auto menuItems = menuItemDAO->getMenuItemByMenuId(menu.menuId);
  std::vector<DTO::FoodItem> foodItems;
  // for (auto &menuItem : menuItems) {
  //   foodItems.push_back(foodItemDAO->getFoodItemById(menuItem.foodItemId));
  // }
  Menu menu{0, (std::string) "Recommended Menu", menuType.menuTypeId};
  return std::make_pair(menu, foodItems);
}

std::vector<UserResponse> ChefService::getUserPreferences(uint64_t chefUserId,
                                                          MenuType menuType,
                                                          std::string date) {
  auto role = getRole(chefUserId);
  if (role.roleName != (std::string) "Chef") {
    throw std::runtime_error("User does not have permission to create menu");
  }
  auto scheduledMenu = scheduledDAO->getScheduledMenuByDate(date);

  std::vector<DTO::Menu> menus;
  for (auto &menu : scheduledMenu) {
    menus.push_back(menuDAO->getMenu(menu.menuId));
  }

  std::vector<DTO::Menu> filteredMenus;
  for (auto &menu : menus) {
    if (menu.menuTypeId == menuType.menuTypeId) {
      filteredMenus.push_back(menu);
      break;
    }
  }

  std::vector<DTO::FoodItem> foodItems;
  for (auto &menu : filteredMenus) {
    auto menuItems = menuItemDAO->getMenuItemByMenuId(menu.menuId);
    for (auto &menuItem : menuItems) {
      foodItems.push_back(foodItemDAO->getFoodItemById(menuItem.foodItemId));
    }
  }

  std::vector<UserResponse> userResponses;
  for (auto &foodItem : foodItems) {
    auto preference =
        preferenceDAO->getPreferencesByFoodItemId(foodItem.foodItemId);
    uint32_t numberOfLikes = 0;
    uint32_t numberOfDislikes = 0;
    for (auto &p : preference) {
      if (p.preference) {
        numberOfLikes++;
      } else {
        numberOfDislikes++;
      }
    }
    uint32_t numberofVotes = preference.size();
    userResponses.emplace_back(foodItem.foodItemId, numberofVotes,
                               numberOfLikes, numberOfDislikes);
  }
  return userResponses;
}