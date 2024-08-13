#include "Client/AdminHandler.h"
#include "Category.h"
#include "Client/ClientCommunicator.h"
#include "Client/InputHandler.h"
#include "Config.h"
#include "FoodItem.h"
#include "Role.h"
#include "SerializableTypes/Pair.h"
#include <algorithm>
#include <ios>
#include <stdexcept>

void AdminHandler::displayFoodItems(std::vector<DTO::FoodItem> &foodItems) {
  std::cout
      << "\n........................Food Items...........................\n";
  for (auto &foodItem : foodItems) {
    std::cout << "\nItem Id : " << foodItem.foodItemId << "\n";
    std::cout << "Name: " << (std::string)foodItem.itemName << "\n"
              << "Category: "
              << DTO::CategoryToString(
                     (DTO::Category)(uint64_t)foodItem.foodItemTypeId)
              << "\n"
              << "Price: " << foodItem.price << "\n"
              << "Availability : " << std::boolalpha
              << foodItem.availabilityStatus << "\n\n\n";
  }
  std::cout << "\n..............End of Food Item.........................\n";
}

std::vector<DTO::FoodItem> AdminHandler::getFoodItems() {
  DTO::Category category = getCategoryInput();
  std::vector<unsigned char> payload;
  U64 categoryValue = (uint64_t)category;
  payload = categoryValue.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/getFoodItems", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::FoodItem> foodItems;
    foodItems.deserialize(response.second);
    std::vector<DTO::FoodItem> foodItemsVector = foodItems;
    return foodItemsVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch food items, due to : " +
                     (std::string)responseString);
}

void AdminHandler::showFoodItems() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  displayFoodItems(foodItems);
}

void AdminHandler::addFoodItem() {
  InputHandler inputHandler;
  std::string itemName;
  DTO::Category category;
  double price;
  bool availabilityStatus;
  std::cout << "Enter item name: ";
  std::getline(std::cin >> std::ws, itemName);
  category = getCategoryInput();
  std::cout << "Enter price: ";
  price = inputHandler.getInput<double>(
      [](const double &input) { return input > 0; });
  std::cout
      << "Enter availability status (1 for available, 0 for not available): ";
  availabilityStatus = inputHandler.getInput<bool>(
      [](const bool &input) { return input == 0 || input == 1; });
  DTO::FoodItem foodItem(0, price, availabilityStatus, false,
                         (uint64_t)category, itemName);
  std::vector<unsigned char> payload = foodItem.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/addFoodItem", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food item added successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to add food item, due to : "
              << (std::string)responseString << std::endl;
  }
}

void AdminHandler::updateFoodItem() {
  auto foodItems = getFoodItems();
  displayFoodItems(foodItems);
  InputHandler inputHandler;
  std::cout << "Enter item id to update: ";
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  auto foodItem = std::find_if( // NOLINT
      foodItems.begin(), foodItems.end(),
      [foodItemId](const DTO::FoodItem &item) {
        return item.foodItemId == foodItemId;
      });
  if (foodItem == foodItems.end()) {
    std::cout << "Invalid item id\n";
    return;
  }
  DTO::FoodItem updatedFoodItem = *foodItem;
  std::string itemName;
  DTO::Category category;
  double price;
  bool availabilityStatus;
  std::cout << "Enter new item name: ";
  std::getline(std::cin >> std::ws, itemName);
  category = getCategoryInput();
  std::cout << "Enter new price: ";
  price = inputHandler.getInput<double>(
      [](const double &input) { return input > 0; });
  std::cout << "Enter new availability status (1 for available, 0 for not "
               "available): ";
  availabilityStatus = inputHandler.getInput<bool>( // NOLINT
      [](const bool &input) { return input == 0 || input == 1; });
  updatedFoodItem.itemName = itemName;
  updatedFoodItem.foodItemTypeId = (uint64_t)category;
  updatedFoodItem.price = price;
  updatedFoodItem.availabilityStatus = availabilityStatus;
  std::vector<unsigned char> payload = updatedFoodItem.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/updateFoodItem", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food item updated successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to update food item, due to : "
              << (std::string)responseString << std::endl;
  }
}

void AdminHandler::deleteFoodItem() {
  auto foodItems = getFoodItems();
  displayFoodItems(foodItems);
  InputHandler inputHandler;
  std::cout << "Enter item id to delete: ";
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  auto foodItem = std::find_if( // NOLINT
      foodItems.begin(), foodItems.end(),
      [foodItemId](const DTO::FoodItem &item) {
        return item.foodItemId == foodItemId;
      });
  if (foodItem == foodItems.end()) {
    std::cout << "Invalid item id\n";
    return;
  }
  std::vector<unsigned char> payload = foodItemId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/removeFoodItem", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food item deleted successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to delete food item, due to : "
              << (std::string)responseString << std::endl;
  }
}

void AdminHandler::handleUserSelection(int choice) {
  switch (choice) {
  case 1:
    showFoodItems();
    break;
  case 2:
    addFoodItem();
    break;
  case 3:
    updateFoodItem();
    break;
  case 4:
    deleteFoodItem();
    break;
  case 5:
    viewNotifications("/Admin/viewNotifications");
    break;
  case 6:
    setFoodItemAttribute();
    break;
  case 7:
    getFoodItemAttribute();
    break;
  case 8:
    removeFoodItemAttribute();
    break;
  case 9:
    addAttribute();
    break;
  case 10:
    logout();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
  }
}

void AdminHandler::performAction() {
  if (isLoggedIn || !login()) {
    return;
  }
  int choice;
  InputHandler inputHandler;
  do {
    std::cout << "1. Show Food Items\n"
              << "2. Add Food Item\n"
              << "3. Update Food Item\n"
              << "4. Delete Food Item\n"
              << "5. view Notifications\n"
              << "6. set FoodItem Attribute\n"
              << "7. get FoodItem Attribute\n"
              << "8. remove FoodItem Attribute\n"
              << "9. Add Attribute\n"
              << "10. Logout\n"
              << "Enter your choice: ";
    choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 10; });
    handleUserSelection(choice);
  } while (choice != 10);
}

AdminHandler::AdminHandler()
    : UserHandler(DTO::Role::Admin), isLoggedIn(false) {}

bool AdminHandler::login() {
  std::cout << "Admin login\n";
  bool result = UserHandler::login();
  isLoggedIn = result;
  return result;
}

bool AdminHandler::logout() {
  std::cout << "Admin logout\n";
  bool result = UserHandler::logout();
  isLoggedIn = !result;
  return result;
}

std::vector<std::pair<uint64_t, std::string>> AdminHandler::getAllAttributes() {
  std::vector<unsigned char> payload;
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/getAllAttributes", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<U64, SString>> attributes;
    attributes.deserialize(response.second);
    std::vector<std::pair<uint64_t, std::string>> attributesVector;
    for (int i = 0; i < attributes.NumberOfItems(); i++) {
      attributesVector.push_back(
          {attributes[i].first, (std::string)attributes[i].second});
    }
    return attributesVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch attributes, due to : " +
                     (std::string)responseString);
}

void AdminHandler::setFoodItemAttribute() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  displayFoodItems(foodItems);
  InputHandler inputHandler;
  std::cout << "Enter item id to set attribute: ";
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  auto foodItem = std::find_if( // NOLINT
      foodItems.begin(), foodItems.end(),
      [foodItemId](const DTO::FoodItem &item) {
        return item.foodItemId == foodItemId;
      });
  if (foodItem == foodItems.end()) {
    std::cout << "Invalid item id\n";
    return;
  }
  std::vector<std::pair<uint64_t, std::string>> attributes = getAllAttributes();
  displayFoodItemPreferences(attributes);
  std::vector<uint64_t> selectedAttributes =
      getFoodItemAttributeSelection(attributes);
  std::vector<unsigned char> payload;
  payload = foodItemId.serialize();
  for (auto &attribute : selectedAttributes) {
    if (!updateFoodItemAttribute(foodItemId, attribute,
                                 "/Admin/addFoodAttribute")) {
      break;
    }
  }
}

bool AdminHandler::updateFoodItemAttribute(U64 foodItemId, U64 attributeId,
                                           const std::string &endpoint) {
  std::vector<unsigned char> payload;
  Pair<U64, U64> data;
  data.first = foodItemId;
  data.second = attributeId;
  payload = data.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId, endpoint,
                                 payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food item attribute updated successfully\n";
    return true;
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to update food item attribute, due to : "
              << (std::string)responseString << std::endl;
    return false;
  }
}

void AdminHandler::getFoodItemAttribute() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  displayFoodItems(foodItems);
  InputHandler inputHandler;
  std::cout << "Enter item id to get attribute: ";
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  auto foodItem = std::find_if( // NOLINT
      foodItems.begin(), foodItems.end(),
      [foodItemId](const DTO::FoodItem &item) {
        return item.foodItemId == foodItemId;
      });
  if (foodItem == foodItems.end()) {
    std::cout << "Invalid item id\n";
    return;
  }
  std::vector<unsigned char> payload;
  payload = foodItemId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/viewAttributes", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<U64, SString>> attributes;
    attributes.deserialize(response.second);
    std::vector<std::pair<uint64_t, std::string>> attributesVector;
    for (int i = 0; i < attributes.NumberOfItems(); i++) {
      attributesVector.push_back(
          {attributes[i].first, (std::string)attributes[i].second});
    }
    // display attributes
    displayFoodItemPreferences(attributesVector);
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to get food item attribute, due to : "
              << (std::string)responseString << std::endl;
  }
}

void AdminHandler::removeFoodItemAttribute() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  displayFoodItems(foodItems);
  InputHandler inputHandler;
  std::cout << "Enter item id to remove attribute: ";
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  auto foodItem = std::find_if( // NOLINT
      foodItems.begin(), foodItems.end(),
      [foodItemId](const DTO::FoodItem &item) {
        return item.foodItemId == foodItemId;
      });
  if (foodItem == foodItems.end()) {
    std::cout << "Invalid item id\n";
    return;
  }
  std::vector<std::pair<uint64_t, std::string>> attributes = getAllAttributes();
  std::vector<uint64_t> selectedAttributes =
      getFoodItemAttributeSelection(attributes);
  std::vector<unsigned char> payload;
  payload = foodItemId.serialize();
  for (auto &attribute : selectedAttributes) {
    if (!updateFoodItemAttribute(foodItemId, attribute,
                                 "/Admin/removeAttribute")) {
      break;
    }
  }
}

void AdminHandler::addAttribute() {
  auto attributes = getAllAttributes();
  // display attributes
  std::cout << "Existing attributes\n\n";
  for (auto &attribute : attributes) {
    std::cout << "Attribute Id: " << attribute.first
              << " Attribute Name: " << attribute.second << std::endl;
  }
  std::cout << "\n\n\n";
  InputHandler inputHandler;
  std::string attributeName;
  std::cout << "Enter attribute name: ";
  std::getline(std::cin >> std::ws, attributeName);
  // check if attribute already exists
  auto attribute = std::find_if( // NOLINT
      attributes.begin(), attributes.end(),
      [attributeName](const std::pair<uint64_t, std::string> &attr) {
        return attr.second == attributeName;
      });
  if (attribute != attributes.end()) {
    std::cout << "Attribute already exists\n";
    return;
  }
  std::cout << "\n\n\n";
  addAttribte(attributeName);
}

void AdminHandler::addAttribte(const std::string &attributeName) {
  std::vector<unsigned char> payload;
  SString attribute(attributeName);
  payload = attribute.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Admin/addAttribute", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Attribute added successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to add attribute, due to : "
              << (std::string)responseString << std::endl;
  }
}