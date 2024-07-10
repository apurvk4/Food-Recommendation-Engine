#include "Client/UserHandler.h"
#include "Category.h"
#include "Notification.h"
#include <chrono>
#include <iomanip>

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

std::string UserHandler::getDate(int64_t hourOffset) {

  auto now = std::chrono::system_clock::now();

  auto tomorrow = now + std::chrono::hours(hourOffset);

  std::time_t tt = std::chrono::system_clock::to_time_t(tomorrow);
  std::tm local_tm = *std::localtime(&tt);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y-%m-%d");

  return oss.str();
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
              << " Food Item Name: " << foodItemPreference.second << std::endl;
  }
  std::cout << "\n...................End of Food Item "
               "Preferences........................\n";
}

void UserHandler::showFoodItems(const std::vector<DTO::FoodItem> &foodItems) {
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