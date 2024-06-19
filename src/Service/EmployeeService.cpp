#include "Service/EmployeeService.h"
#include "DTO/Feedback.h"
#include "DTO/ScheduledMenu.h"
#include "Service/UserService.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>

std::string getCurrentDateWithMidnightTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);
  now_tm.tm_hour = 0;
  now_tm.tm_min = 0;
  now_tm.tm_sec = 0;
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

using DTO::Menu;
using DTO::Role;
using Service::EmployeeService;
using Service::UserService;

bool EmployeeService::verifyRole(uint64_t userId) {
  auto role = getRole(userId);
  return role.roleName == "Employee";
}

EmployeeService::EmployeeService(
    std::shared_ptr<IUserDAO> userDAO, std::shared_ptr<IRoleDAO> roleDAO,
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

std::pair<DTO::Menu, std::vector<DTO::FoodItem>>
EmployeeService::getTodaysMenu(uint64_t userId, uint64_t menuType) {
  Role role = getRole(userId);
  if (roleDAO->getRoleId("Employee") != role.roleId) {
    throw std::runtime_error("User does not have permission to view menu");
  }
  auto scheduledMenus = scheduledMenuDAO->getScheduledMenuByDate(
      getCurrentDateWithMidnightTime());
  if (scheduledMenus.size() == 0) {
    throw std::runtime_error("No menu scheduled for today");
  }
  // filter out the survey menu
  scheduledMenus.erase(std::remove_if(scheduledMenus.begin(),
                                      scheduledMenus.end(),
                                      [](const DTO::ScheduledMenu &menu) {
                                        return menu.isSurveyMenu == true;
                                      }),
                       scheduledMenus.end());

  int index = -1;
  for (int i = 0; i < scheduledMenus.size(); i++) {
    auto menuTyeId = menuDAO->getMenuType(scheduledMenus[i].menuId);
    if (menuTyeId == menuType) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    throw std::runtime_error(
        "today's menu of given menuType not rolled out yet");
  }
  auto menu = menuDAO->getMenu(scheduledMenus[index].menuId);
  auto menuItems =
      menuItemDAO->getMenuItemByMenuId(scheduledMenus[index].menuId);
  std::vector<DTO::FoodItem> foodItems;
  for (auto &menuItem : menuItems) {
    auto foodItem = foodItemDAO->getFoodItemById(menuItem.foodItemId);
    foodItems.push_back(foodItem);
  }
  return std::make_pair(menu, foodItems);
}

bool EmployeeService::sendFeedback(uint64_t foodItemId, uint64_t menuId,
                                   uint64_t userId, std::string comment,
                                   int rating) {
  Role role = getRole(userId);
  if (roleDAO->getRoleId("Employee") != role.roleId) {
    throw std::runtime_error("User does not have permission to view menu");
  }
  auto user = userDAO->getUserById(userId);
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  auto date = oss.str();
  DTO::Feedback feedback(0, userId, menuId, foodItemId, rating, comment, date);
  return feedbackDAO->addFeedback(feedback);
}

std::pair<DTO::ScheduledMenu, std::vector<DTO::FoodItem>>
EmployeeService::getNextMenuRollout(uint64_t userId, uint64_t menuType) {
  Role role = getRole(userId);
  if (roleDAO->getRoleId("Employee") != role.roleId) {
    throw std::runtime_error("User does not have permission to view menu");
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
  auto scheduledMenu = scheduledMenuDAO->getScheduledMenuByDate(date);
  if (scheduledMenu.size() == 0) {
    throw std::runtime_error("Menu not rolled out yet");
  }
  // filter out the non-survey menu
  scheduledMenu.erase(std::remove_if(scheduledMenu.begin(), scheduledMenu.end(),
                                     [](const DTO::ScheduledMenu &menu) {
                                       return menu.isSurveyMenu == false;
                                     }),
                      scheduledMenu.end());
  if (scheduledMenu.size() == 0) {
    throw std::runtime_error("No survey menu scheduled for tomorrow");
  }
  int index = -1;
  int i = 0;
  for (auto &scheduled : scheduledMenu) {
    uint64_t scheduledMenuType = menuDAO->getMenuType(scheduled.menuId);
    if (menuType == scheduledMenuType) {
      index = i;
      break;
    }
    i++;
  }
  if (index == -1) {
    throw std::runtime_error(
        "no menu rolled out till now for next day for the menuType given");
  }
  auto menuItems =
      menuItemDAO->getMenuItemByMenuId(scheduledMenu[index].menuId);
  std::vector<DTO::FoodItem> foodItems;
  for (auto &menuItem : menuItems) {
    auto foodItem = foodItemDAO->getFoodItemById(menuItem.foodItemId);
    foodItems.push_back(foodItem);
  }
  return std::make_pair(scheduledMenu[index], foodItems);
}

bool EmployeeService::setNextMenuChoice(
    uint64_t userId, uint64_t scheduledMenuId,
    std::unordered_map<uint64_t, bool> foodItemChoices) {
  auto scheduledMenu = scheduledMenuDAO->getScheduledMenu(scheduledMenuId);
  for (auto choice : foodItemChoices) {
    preferenceDAO->addPreference(userId, choice.first, scheduledMenu.menuId,
                                 choice.second);
  }
  return true;
}
