#pragma once

#include "Category.h"
#include "Client/ClientCommunicator.h"
#include "Client/InputHandler.h"
#include "FoodItem.h"
#include "Notification.h"
#include "Role.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "User.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

class UserHandler {
protected:
  const DTO::Role roleId;
  DTO::User user;
  void displayNotification(std::vector<DTO::Notification> &notifications) {
    std::cout << "\n........Notifications............\n";
    if (notifications.size() <= 0) {
      std::cout << "No notifications\n";
      std::cout << "\n................................\n";
      return;
    }
    for (auto &notification : notifications) {
      std::cout << "\n--------------------------------\n";
      std::cout << (std::string)notification.message << std::endl;
      std::cout << "--------------------------------\n";
    }
  }

public:
  UserHandler(DTO::Role roleId) : roleId(roleId) {}
  virtual void performAction() = 0;
  void displayFoodItems(const std::vector<DTO::FoodItem> &foodItems);
  DTO::Category getCategoryInput();
  std::string getDateInput();
  virtual bool login();
  virtual bool logout();
  virtual bool viewNotifications(const std::string &endpoint);
  std::vector<uint64_t> getFoodItemAttributeSelection(
      const std::vector<std::pair<uint64_t, std::string>> &attributes);
  void displayFoodItemPreferences(
      const std::vector<std::pair<uint64_t, std::string>> &foodItemPreferences);
  virtual ~UserHandler() = default;
};