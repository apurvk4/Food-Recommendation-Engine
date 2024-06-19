#pragma once

#include "DAO/IFeedbackDAO.h"
#include "DAO/IFoodItemDAO.h"
#include "DAO/IMenuDAO.h"
#include "DAO/IMenuItemDAO.h"
#include "DAO/INotificationDAO.h"
#include "DAO/IPreferenceDAO.h"
#include "DAO/IScheduledMenuDAO.h"
#include "DAO/IUserDAO.h"
#include "DTO/FoodItem.h"
#include "DTO/Menu.h"
#include "DTO/Notification.h"
#include "DTO/ScheduledMenu.h"
#include "EmployeeService.h"
#include "UserService.h"
#include <unordered_map>

namespace Service {
using DAO::IFeedbackDAO;
using DAO::IFoodItemDAO;
using DAO::IMenuDAO;
using DAO::IMenuItemDAO;
using DAO::INotificationDAO;
using DAO::IPreferenceDAO;
using DAO::IRoleDAO;
using DAO::IScheduledMenuDAO;
using DAO::IUserDAO;
using DTO::Menu;
using DTO::Notification;
using DTO::User;
using Service::UserService;

class EmployeeService : public UserService {
  std::shared_ptr<IScheduledMenuDAO> scheduledDAO;
  std::shared_ptr<IScheduledMenuDAO> scheduledMenuDAO;
  std::shared_ptr<IMenuDAO> menuDAO;
  std::shared_ptr<IFoodItemDAO> foodItemDAO;
  std::shared_ptr<IMenuItemDAO> menuItemDAO;
  std::shared_ptr<IFeedbackDAO> feedbackDAO;
  std::shared_ptr<IPreferenceDAO> preferenceDAO;
  bool verifyRole(uint64_t userId);

public:
  EmployeeService(std::shared_ptr<IUserDAO> userDAO,
                  std::shared_ptr<IRoleDAO> roleDAO,
                  std::shared_ptr<INotificationDAO> notificationDAO,
                  std::shared_ptr<IScheduledMenuDAO> scheduledDAO,
                  std::shared_ptr<IScheduledMenuDAO> scheduledMenuDAO,
                  std::shared_ptr<IMenuDAO> menuDAO,
                  std::shared_ptr<IFoodItemDAO> foodItemDAO,
                  std::shared_ptr<IMenuItemDAO> menuItemDAO,
                  std::shared_ptr<IFeedbackDAO> feedbackDAO,
                  std::shared_ptr<IPreferenceDAO> preferenceDAO);
  std::pair<DTO::Menu, std::vector<DTO::FoodItem>>
  getTodaysMenu(uint64_t userId, uint64_t menuType);
  bool sendFeedback(uint64_t foodItemId, uint64_t menuId, uint64_t userId,
                    std::string comment, int rating);
  std::pair<DTO::ScheduledMenu, std::vector<DTO::FoodItem>>
  getNextMenuRollout(uint64_t userId, uint64_t menuType);
  bool setNextMenuChoice(uint64_t userId, uint64_t scheduledMenu,
                         std::unordered_map<uint64_t, bool> foodItemChoices);
};
}; // namespace Service