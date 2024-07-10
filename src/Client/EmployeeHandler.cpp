#include "Client/EmployeeHandler.h"
#include "Category.h"
#include "Client/ClientCommunicator.h"
#include "Client/InputHandler.h"
#include "Client/TcpClient.h"
#include "Client/UserHandler.h"
#include "DiscardFeedbackAnswer.h"
#include "Feedback.h"
#include "FoodItem.h"
#include "Menu.h"
#include "Review.h"
#include "Role.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include <stdexcept>

std::pair<DTO::Menu, std::vector<DTO::FoodItem>>
EmployeeHandler::getMenu(U64 categoryId, SString date) {
  Pair<U64, SString> request;
  request.first = categoryId;
  request.second = date;
  std::vector<unsigned char> payload = request.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/viewMenu", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<DTO::Menu, Array<DTO::FoodItem>>> responseArray;
    responseArray.deserialize(response.second);
    std::vector<Pair<DTO::Menu, Array<DTO::FoodItem>>> menuData = responseArray;
    std::vector<std::pair<DTO::Menu, std::vector<DTO::FoodItem>>>
        menuWithFoodItems;
    for (auto &menu : menuData) {
      std::vector<DTO::FoodItem> foodItems = menu.second;
      menuWithFoodItems.push_back(std::make_pair(menu.first, foodItems));
    }
    if (menuWithFoodItems.size() > 0) {
      return menuWithFoodItems[0];
    }
    throw std::runtime_error("No menu available for today");
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

std::pair<DTO::Menu, std::vector<std::pair<double, DTO::FoodItem>>>
EmployeeHandler::getRolloutMenu(U64 categoryId, SString date) {
  Pair<U64, SString> request;
  request.first = categoryId;
  request.second = date;
  std::vector<unsigned char> payload = request.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/viewMenuRollout", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Pair<DTO::Menu, Array<Pair<Double, DTO::FoodItem>>> responseObject;
    responseObject.deserialize(response.second);
    DTO::Menu menu = responseObject.first;
    std::vector<Pair<Double, DTO::FoodItem>> foodItemsRating =
        responseObject.second;
    std::vector<std::pair<double, DTO::FoodItem>> foodItemsWithRating;
    for (auto &foodItemPair : foodItemsRating) {
      DTO::FoodItem foodItem = foodItemPair.second;
      foodItemsWithRating.push_back(
          std::make_pair(foodItemPair.first, foodItem));
    }
    return std::make_pair(menu, foodItemsWithRating);
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

void EmployeeHandler::showTodaysMenu() {
  DTO::Category category = getCategoryInput();
  std::cout << "...............Menu for today......................\n";
  try {
    std::pair<DTO::Menu, std::vector<DTO::FoodItem>> menu =
        getMenu((uint64_t)category, getDate(0));
    displayMenu(menu);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "\n...............End of Menu for today......................\n";
}

void EmployeeHandler::displayMenu(
    const DTO::Menu menu,
    std::vector<std::pair<double, DTO::FoodItem>> foodItemWithRating) {
  std::cout << "\n...............Menu for today......................\n";
  if (foodItemWithRating.size() == 0) {
    std::cout << "No menu available for today\n";
    std::cout
        << "\n...............End of Menu for today......................\n";
    return;
  }
  std::cout << "Menu Id: " << menu.menuId << std::endl;
  std::cout << "Menu Name: " << (std::string)menu.menuName << std::endl;
  std::cout << "Menu Category: "
            << DTO::CategoryToString((DTO::Category)(uint64_t)menu.categoryId)
            << std::endl;
  std::cout << "Menu Date: " << (std::string)menu.date << std::endl;
  std::cout << "Menu Items\n";
  for (auto &foodItem : foodItemWithRating) {
    std::cout << "\n--------------------------------\n";
    std::cout << "Item Id: " << foodItem.second.foodItemId << std::endl;
    std::cout << "Item Name: " << (std::string)foodItem.second.itemName
              << std::endl;
    std::cout << "Item Price: " << foodItem.second.price << std::endl;
    std::cout << "Item Category: "
              << DTO::CategoryToString(
                     (DTO::Category)(uint64_t)foodItem.second.foodItemTypeId)
              << std::endl;
    std::cout << "Item Rating: " << foodItem.first << std::endl;
    std::cout << "\n--------------------------------\n";
  }
  std::cout << "\n...............End of Menu for today......................\n";
}

void EmployeeHandler::showNextMenuRollout() {
  DTO::Category category = getCategoryInput();
  try {
    std::pair<DTO::Menu, std::vector<std::pair<double, DTO::FoodItem>>> menu =
        getRolloutMenu((uint64_t)category, getDate(24));
    displayMenu(menu.first, menu.second);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void EmployeeHandler::provideRolloutFeedback() {
  DTO::Category category = getCategoryInput();
  auto result = getRolloutMenu((uint64_t)category, getDate(24));
  displayMenu(result.first, result.second);
  for (auto &foodItemRatingPair : result.second) {
    std::cout << "Enter preference(0 or 1) for "
              << (std::string)foodItemRatingPair.second.itemName << ": ";
    InputHandler inputHandler;
    bool preference = inputHandler.getInput<int>(
        [](const int &input) { return input >= 0 && input <= 1; });
    DTO::Feedback feedback;
    feedback.foodItemId = foodItemRatingPair.second.foodItemId;
    feedback.preference = preference;
    feedback.userId = user.userId;
    feedback.date = getDate(0);

    std::vector<unsigned char> payload = feedback.serialize();
    ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
    clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                   "/Employee/sendFeedback", payload);
    auto response = clientCommunicator.receiveResponse();
    if (response.first == 0) {
      std::cout << "Feedback submitted successfully\n";
    } else {
      SString error;
      error.deserialize(response.second);
      std::cout << "Error : " << (std::string)error << std::endl;
    }
  }
}

void EmployeeHandler::addReview() {
  DTO::Category category = getCategoryInput();
  auto result = getMenu((uint64_t)category, getDate(0));
  displayMenu(result);
  for (auto &foodItem : result.second) {
    std::cout << "Enter rating(1-5) for " << (std::string)foodItem.itemName
              << ": ";
    InputHandler inputHandler;
    int rating = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 5; });
    std::cout << "Enter comment for " << (std::string)foodItem.itemName << ": ";
    std::string comment;
    std::getline(std::cin >> std::ws, comment);
    DTO::Review review;
    review.foodItemId = foodItem.foodItemId;
    review.rating = rating;
    review.comment = comment;
    review.userId = user.userId;
    review.date = getDate(0);

    std::vector<unsigned char> payload = review.serialize();
    ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
    clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                   "/Employee/writeReview", payload);
    auto response = clientCommunicator.receiveResponse();
    if (response.first == 0) {
      std::cout << "Review submitted successfully\n";
    } else {
      SString error;
      error.deserialize(response.second);
      std::cout << "Error : " << (std::string)error << std::endl;
    }
  }
}

void EmployeeHandler::handleUserSelection(int choice) {
  switch (choice) {
  case 1:
    showTodaysMenu();
    break;
  case 2:
    showNextMenuRollout();
    break;
  case 3:
    provideRolloutFeedback();
    break;
  case 4:
    addReview();
    break;
  case 5:
    viewNotifications("/Employee/viewNotifications");
    break;
  case 6:
    setFoodItemPreference();
    break;
  case 7:
    showFoodItemPreferences();
    break;
  case 8:
    deleteFoodItemPreference();
    break;
  case 9:
    showDiscardedFoodItems();
    break;
  case 10:
    addDiscardFeedbackAnswer();
    break;
  case 11:
    logout();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
  }
}

void EmployeeHandler::displayMenu(
    const std::pair<DTO::Menu, std::vector<DTO::FoodItem>> &menus) {
  std::cout << "\n...............Menu for today......................\n";
  std::cout << "Menu Id: " << menus.first.menuId << std::endl;
  std::cout << "Menu Name: " << (std::string)menus.first.menuName << std::endl;
  std::cout << "Menu Category: "
            << DTO::CategoryToString(
                   (DTO::Category)(uint64_t)menus.first.categoryId)
            << std::endl;
  std::cout << "Menu Date: " << (std::string)menus.first.date << std::endl;
  std::cout << "Menu Items\n";
  for (auto &foodItem : menus.second) {
    std::cout << "\n--------------------------------\n";
    std::cout << "Item Id: " << foodItem.foodItemId << std::endl;
    std::cout << "Item Name: " << (std::string)foodItem.itemName << std::endl;
    std::cout << "Item Price: " << foodItem.price << std::endl;
    std::cout << "Item Category: "
              << DTO::CategoryToString(
                     (DTO::Category)(uint64_t)foodItem.foodItemTypeId)
              << std::endl;
    std::cout << "\n--------------------------------\n";
  }
  std::cout << "\n...............End of Menu for today......................\n";
}

void EmployeeHandler::performAction() {
  if (isLoggedIn || !login()) {
    return;
  }
  int choice;
  InputHandler inputHandler;
  do {
    std::cout << "1. Show Todays Menu\n"
              << "2. View Next Menu Rollout\n"
              << "3. Provide Rollout Feedback\n"
              << "4. Add Review\n"
              << "5. view Notification\n"
              << "6. set Food Preferences\n"
              << "7. get Food Preferences\n"
              << "8. remove Food Preferences\n"
              << "9. Show Discarded Food Items\n"
              << "10. Provide Discard Feedback\n"
              << "11. Logout\n"
              << "Enter your choice: ";
    choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 11; });
    handleUserSelection(choice);
  } while (choice != 11);
}

EmployeeHandler::EmployeeHandler()
    : UserHandler(DTO::Role::Employee), isLoggedIn(false) {}

bool EmployeeHandler::login() {
  std::cout << "Employee login\n";
  bool result = UserHandler::login();
  isLoggedIn = result;
  return result;
}

bool EmployeeHandler::logout() {
  std::cout << "Employee logout\n";
  bool result = UserHandler::logout();
  isLoggedIn = result;
  return result;
}

std::vector<std::pair<uint64_t, std::string>>
EmployeeHandler::getAllAttributes() {
  std::vector<std::pair<uint64_t, std::string>> attributes;
  std::vector<unsigned char> payload;
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/getAttributes", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<U64, SString>> responseArray;
    responseArray.deserialize(response.second);
    std::vector<Pair<U64, SString>> attributesData = responseArray;
    for (auto &attribute : attributesData) {
      attributes.push_back(
          std::make_pair(attribute.first, (std::string)attribute.second));
    }
    return attributes;
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

std::vector<std::pair<uint64_t, std::string>>
EmployeeHandler::getFoodItemPreferences() {
  std::vector<std::pair<uint64_t, std::string>> foodItemPreferences;
  U64 userId = user.userId;
  std::vector<unsigned char> payload = userId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/getFoodPreferences", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<U64, SString>> responseArray;
    responseArray.deserialize(response.second);
    for (int i = 0; i < responseArray.NumberOfItems(); i++) {
      foodItemPreferences.push_back(std::make_pair(
          responseArray[i].first, (std::string)responseArray[i].second));
    }
    return foodItemPreferences;
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

void EmployeeHandler::showFoodItemPreferences() {
  try {
    std::vector<std::pair<uint64_t, std::string>> foodItemPreferences =
        getFoodItemPreferences();
    displayFoodItemPreferences(foodItemPreferences);
  } catch (std::exception &e) {
    std::cout << "Error getting food Item Preferences : " << e.what()
              << std::endl;
  }
}

void EmployeeHandler::deleteFoodItemPreference() {
  std::vector<std::pair<uint64_t, std::string>> foodItemPreferences;
  try {
    foodItemPreferences = getFoodItemPreferences();
    displayFoodItemPreferences(foodItemPreferences);
  } catch (std::exception &e) {
    std::cout << "Error getting food Item Preferences : " << e.what()
              << std::endl;
    return;
  }
  Array<U64> selectionArray;
  std::vector<uint64_t> selection =
      getFoodItemAttributeSelection(foodItemPreferences);
  for (auto &attributeId : selection) {
    selectionArray.push_back(attributeId);
  }
  std::vector<unsigned char> payload = selectionArray.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/deletePreferences", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food Item preference deleted successfully\n";
  } else {
    SString error;
    error.deserialize(response.second);
    std::cout << "Error : " << (std::string)error << std::endl;
  }
}

void EmployeeHandler::setFoodItemPreference() {
  std::vector<std::pair<uint64_t, std::string>> attributes;
  try {
    attributes = getAllAttributes();
    displayFoodItemPreferences(attributes);
  } catch (std::exception &e) {
    std::cout << "Error getting attributes : " << e.what() << std::endl;
    return;
  }
  Array<U64> selectionArray;
  std::vector<uint64_t> selection = getFoodItemAttributeSelection(attributes);
  for (auto &attributeId : selection) {
    selectionArray.push_back(attributeId);
  }
  std::vector<unsigned char> payload = selectionArray.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/addFoodPreferences", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food Item preference set successfully\n";
  } else {
    SString error;
    error.deserialize(response.second);
    std::cout << "Error : " << (std::string)error << std::endl;
  }
}

std::vector<DTO::FoodItem> EmployeeHandler::getDiscardedFoodItems() {
  std::vector<DTO::FoodItem> discardedFoodItems;
  std::vector<unsigned char> payload;
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/getDiscardedItem", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::FoodItem> responseArray;
    responseArray.deserialize(response.second);
    discardedFoodItems = responseArray;
    return discardedFoodItems;
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

std::vector<DTO::DiscardFeedbackQuestion>
EmployeeHandler::getDiscardFeedbackQuestions(U64 foodItemId) {
  std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions;
  std::vector<unsigned char> payload = foodItemId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Employee/getQuestions", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::DiscardFeedbackQuestion> responseArray;
    responseArray.deserialize(response.second);
    discardFeedbackQuestions = responseArray;
    return discardFeedbackQuestions;
  }
  SString error;
  error.deserialize(response.second);
  throw std::runtime_error("Error : " + (std::string)error);
}

void EmployeeHandler::showDiscardedFoodItems() {
  std::vector<DTO::FoodItem> discardedFoodItems;
  try {
    discardedFoodItems = getDiscardedFoodItems();
    showFoodItems(discardedFoodItems);
  } catch (std::exception &e) {
    std::cout << "Error getting discarded food items : " << e.what()
              << std::endl;
  }
}

void EmployeeHandler::showDiscardFeedbackQuestions() {
  std::vector<DTO::FoodItem> discardedFoodItems;
  try {
    discardedFoodItems = getDiscardedFoodItems();
    showFoodItems(discardedFoodItems);
  } catch (std::exception &e) {
    std::cout << "Error getting discarded food items : " << e.what()
              << std::endl;
    return;
  }
  std::cout << "Enter food item id for which you want to provide feedback: ";
  InputHandler inputHandler;
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions;
  try {
    discardFeedbackQuestions = getDiscardFeedbackQuestions(foodItemId);
    displayDiscardFeedbackQuestions(discardFeedbackQuestions);
  } catch (std::exception &e) {
    std::cout << "Error getting discard feedback questions : " << e.what()
              << std::endl;
  }
}

void EmployeeHandler::displayDiscardFeedbackQuestions(
    const std::vector<DTO::DiscardFeedbackQuestion> &questions) {
  for (auto &question : questions) {
    std::cout << "Question: " << (std::string)question.question << std::endl;
  }
}

void EmployeeHandler::addDiscardFeedbackAnswer() {
  std::vector<DTO::FoodItem> discardedFoodItems;
  try {
    discardedFoodItems = getDiscardedFoodItems();
    showFoodItems(discardedFoodItems);
  } catch (std::exception &e) {
    std::cout << "Error getting discarded food items : " << e.what()
              << std::endl;
    return;
  }
  std::cout << "Enter food item id for which you want to provide feedback: ";
  InputHandler inputHandler;
  U64 foodItemId = inputHandler.getInput<uint64_t>(
      [](const uint64_t &input) { return input > 0; });
  std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions;
  try {
    discardFeedbackQuestions = getDiscardFeedbackQuestions(foodItemId);
    displayDiscardFeedbackQuestions(discardFeedbackQuestions);
  } catch (std::exception &e) {
    std::cout << "Error getting discard feedback questions : " << e.what()
              << std::endl;
    return;
  }
  for (auto &question : discardFeedbackQuestions) {
    std::cout << "Enter answer for question: " << (std::string)question.question
              << ": ";
    std::string answer;
    std::getline(std::cin >> std::ws, answer);
    DTO::DiscardFeedbackAnswer feedbackAnswer;
    feedbackAnswer.questionId = question.questionId;
    feedbackAnswer.userId = user.userId;
    feedbackAnswer.answer = answer;

    std::vector<unsigned char> payload = feedbackAnswer.serialize();
    ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
    clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                   "/Employee/addAnswer", payload);
    auto response = clientCommunicator.receiveResponse();
    if (response.first == 0) {
      std::cout << "Feedback answer submitted successfully\n";
    } else {
      SString error;
      error.deserialize(response.second);
      std::cout << "Error : " << (std::string)error << std::endl;
    }
  }
}