#include "Client/UserHandler.h"
#include "Category.h"
#include "LoginData.h"
#include "Notification.h"

bool UserHandler::viewNotifications(const std::string &endpoint) {
  std::vector<unsigned char> payload = user.userId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId, endpoint,
                                 payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::Notification> notificationArray;
    notificationArray.deserialize(response.second);
    std::vector<DTO::Notification> notifications = notificationArray;
    displayNotification(notifications);
    return true;
  } else {
    std::cout << "Failed to get notifications due to : ";
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << (std::string)responseString << std::endl;
    return false;
  }
}

DTO::Category UserHandler::getCategoryInput() {
  InputHandler inputHandler;
  std::cout << "1. Breakfast\n"
            << "2. Lunch\n"
            << "3. Dinner\n"
            << "Enter your choice: ";
  int choice = inputHandler.getInput<int>(
      [](const int &input) { return input >= 1 && input <= 3; });
  DTO::Category category = DTO::Category::Breakfast;
  switch (choice) {
  case 1:
    category = DTO::Category::Breakfast;
    break;
  case 2:
    category = DTO::Category::Lunch;
    break;
  case 3:
    category = DTO::Category::Dinner;
    break;
  default:
    throw std::runtime_error("Invalid choice. Defaulting to Breakfast\n");
  }
  return category;
}

bool UserHandler::login() {
  InputHandler inputHandler;
  uint64_t userId;
  std::string password;
  std::cout << "Enter user id: ";
  userId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  std::cout << "Enter password: ";
  password = inputHandler.getInput<std::string>(
      [](const std::string &input) { return input.length() > 0; });
  LoginData loginData;
  loginData.userId = userId;
  loginData.password = password;
  loginData.roleId = (uint64_t)roleId;
  std::vector<unsigned char> payload = loginData.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(userId, (uint64_t)roleId, "/Auth/login",
                                 payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Login successful\n";
    user.deserialize(response.second);
    return true;
  } else {
    std::cout << "Login failed due to : ";
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << (std::string)responseString << std::endl;
    return false;
  }
}

bool UserHandler::logout() {
  InputHandler inputHandler;
  std::vector<unsigned char> payload;
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId, "/Auth/logout",
                                 payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Logout successful\n";
    return true;
  } else {
    std::cout << "Logout failed due to : ";
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << (std::string)responseString << std::endl;
    return false;
  }
}

std::vector<uint64_t> UserHandler::getFoodItemAttributeSelection(
    const std::vector<std::pair<uint64_t, std::string>> &attributes) {
  if (attributes.size() == 0) {
    return {};
  }
  std::vector<uint64_t> selectedAttributes;
  InputHandler inputHandler;
  uint64_t attributeId = 0;
  do {
    std::cout << "Enter attribute id to select or press 0 to exit : ";
    attributeId = inputHandler.getInput<uint64_t>([&](const uint64_t &input) {
      if (input == 0) {
        return true;
      }
      for (auto &attribute : attributes) {
        if (attribute.first == input) {
          return true;
        }
      }
      return false;
    });
    if (attributeId != 0) {
      selectedAttributes.push_back(attributeId);
    }
  } while (attributeId != 0);
  return selectedAttributes;
}

void UserHandler::displayFoodItemPreferences(
    const std::vector<std::pair<uint64_t, std::string>> &foodItemPreferences) {
  std::cout
      << "\n...................Food Item Preferences........................\n";
  if (foodItemPreferences.size() == 0) {
    std::cout << "No food item preferences set\n";
  }
  for (auto &foodItemPreference : foodItemPreferences) {
    std::cout << "Food Item Id: " << foodItemPreference.first
              << " Attribute: " << foodItemPreference.second << std::endl;
  }
  std::cout << "\n...................End of Food Item "
               "Preferences........................\n";
}

void UserHandler::displayFoodItems(
    const std::vector<DTO::FoodItem> &foodItems) {
  std::cout << "\n----------------Food Items------------------\n";
  for (auto &foodItem : foodItems) {
    std::cout << "\n-----------------\n";
    std::cout << "Item Id : " << foodItem.foodItemId << "\n";
    std::cout << "Name: " << (std::string)foodItem.itemName << "\n"
              << "Category: "
              << DTO::CategoryToString(
                     (DTO::Category)(uint64_t)foodItem.foodItemTypeId)
              << "\n"
              << "Price: " << foodItem.price << "\n"
              << "Availability : " << std::boolalpha
              << foodItem.availabilityStatus << "\n";
    std::cout << "\n---------------------\n";
  }
}

std::string UserHandler::getDateInput() {
  InputHandler inputHandler;
  std::string date;

  std::cout << "Enter year: ";
  int year = inputHandler.getInput<int>(
      [](const int &input) { return input >= 1900 && input <= 3000; });
  date += std::to_string(year) + "-";
  std::cout << "Enter month: ";
  int month = inputHandler.getInput<int>(
      [](const int &input) { return input >= 1 && input <= 12; });

  if (month < 10) {
    date += "0";
  }
  date += std::to_string(month) + "-";
  std::cout << "Enter day: ";
  int day = inputHandler.getInput<int>([&](const int &input) {
    if (input < 1 || input > 31) {
      return false;
    }
    if (month == 2) {
      if (year % 4 == 0) {
        return input <= 29;
      } else {
        return input <= 28;
      }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
      return input <= 30;
    }
    return true;
  });
  if (day < 10) {
    date += "0";
  }
  date += std::to_string(day);
  return date;
}

void UserHandler::displayNotification(
    std::vector<DTO::Notification> &notifications) {
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