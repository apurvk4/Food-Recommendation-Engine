#pragma once

#include "EmployeeService.h"
#include "Menu.h"
#include "Notification.h"
#include "UserService.h"
#include <unordered_map>

class IFeedbackDAO;
class IMenuDAO;

class EmployeeService : public UserService {
  std::shared_ptr<IMenuDAO> menuDAO;
  std::shared_ptr<IFeedbackDAO> feedbackDAO;
  bool verifyRole(uint64_t userId);

public:
  EmployeeService(std::shared_ptr<IUserDAO> userDAO,
                  std::shared_ptr<IMenuDAO> menuDAO,
                  std::shared_ptr<IFeedbackDAO> feedbackDAO, User activeUser);
  std::vector<Menu> getTodaysMenu();
  bool sendFeedback(uint64_t foodItemId, uint64_t userId, std::string comment,
                    int rating);
  std::vector<Notification> getUnreadNotifications();
  std::vector<Menu> getNextMenuRollout();
  bool setNextMenuChoice(uint64_t menuId,
                         std::unordered_map<uint64_t, bool> foodItemChoices);
};