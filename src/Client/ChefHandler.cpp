#include "Client/ChefHandler.h"
#include "Category.h"
#include "Client/ClientCommunicator.h"
#include "Client/InputHandler.h"
#include "FoodItem.h"
#include "Menu.h"
#include "MenuItem.h"
#include "Role.h"
#include "SerializableTypes/Pair.h"
#include "VotingResult.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <string>

void ChefHandler::performAction() {
  if (isLoggedIn || !login()) {
    return;
  }
  int choice;
  InputHandler inputHandler;
  do {
    std::cout << "1. Show Food Items\n"
              << "2. Show Recommended Food Items\n"
              << "3. create Menu\n"
              << "4. rollout next menu\n"
              << "5. view Feedback\n"
              << "6. view Notifications\n"
              << "7. Get Discarded Food Items\n"
              << "8. Discard Food Item\n"
              << "9. Show Discard Feedback Questions\n"
              << "10. Add Discard Feedback Question\n"
              << "11. Logout\n"
              << "Enter your choice: ";
    choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 11; });
    handleUserSelection(choice);
  } while (choice != 11);
}

void ChefHandler::handleUserSelection(int choice) {
  switch (choice) {
  case 1:
    showFoodItems(getFoodItems());
    break;
  case 2:
    showFoodItemRecommendation();
    break;
  case 3:
    createTodaysMenu();
    break;
  case 4:
    RolloutNextMenu();
    break;
  case 5:
    viewFeedback();
    break;
  case 6:
    viewNotifications("/Chef/viewNotifications");
    break;
  case 7:
    showFoodItems(getDiscardedFoodItems());
    break;
  case 8:
    discardFoodItem();
    break;
  case 9:
    showDiscardFeedbackQuestions();
    break;
  case 10:
    addDiscardFeedbackQuestion();
    break;
  case 11:
    logout();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
  }
}

ChefHandler::ChefHandler() : UserHandler(DTO::Role::Chef), isLoggedIn(false) {}

bool ChefHandler::login() {
  std::cout << "Chef login\n";
  bool result = UserHandler::login();
  isLoggedIn = result;
  return result;
}

bool ChefHandler::logout() {
  std::cout << "Chef logout\n";
  bool result = UserHandler::logout();
  isLoggedIn = result;
  return result;
}

std::vector<DTO::FoodItem> ChefHandler::getFoodItems() {
  DTO::Category category = getCategoryInput();
  std::vector<unsigned char> payload;
  U64 categoryValue = (uint64_t)category;
  payload = categoryValue.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/viewFoodItems", payload);
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

void ChefHandler::showFoodItemRecommendation() {
  std::vector<DTO::FoodItem> foodItems = getFoodItemRecommendation();
  showFoodItems(foodItems);
}

std::vector<DTO::FoodItem> ChefHandler::getFoodItemRecommendation() {
  DTO::Category category = getCategoryInput();
  std::vector<unsigned char> payload;
  U64 categoryValue = (uint64_t)category;
  U32 count = 10;
  Pair<U64, U32> categoryPair(categoryValue, count);
  payload = categoryPair.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/getRecommendedMenu", payload);
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

std::pair<uint64_t, uint64_t>
ChefHandler::getItemIdRange(std::vector<DTO::FoodItem> &foodItems) {
  if (foodItems.size() <= 0) {
    return std::make_pair(0, 0);
  }
  std::pair<uint64_t, uint64_t> range =
      std::make_pair(foodItems[0].foodItemId, foodItems[0].foodItemId);
  for (auto &foodItem : foodItems) {
    range.first = std::min(range.first, (uint64_t)foodItem.foodItemId);
    range.second = std::max(range.second, (uint64_t)foodItem.foodItemId);
  }
  return range;
}

std::vector<DTO::MenuItem> ChefHandler::getMenuItemInput() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  showFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::vector<DTO::MenuItem> menuItems;
  InputHandler inputHandler;
  std::cout << "Enter the item ids to add to menu. Enter 0 to stop\n";
  do {
    std::cout << "Enter item id: ";
    uint64_t itemId =
        inputHandler.getInput<uint64_t>([itemIdRange](const uint64_t &input) {
          return (input >= itemIdRange.first && input <= itemIdRange.second) ||
                 input == 0;
        });
    if (itemId == 0) {
      break;
    }
    auto it = std::find_if(foodItems.begin(), foodItems.end(),
                           [itemId](const DTO::FoodItem &foodItem) {
                             return (uint64_t)foodItem.foodItemId == itemId;
                           });
    std::cout << "Enter quantity : ";
    uint64_t quantity = inputHandler.getInput<uint64_t>(
        [](const uint64_t &input) { return input > 0; });
    menuItems.push_back(DTO::MenuItem(0, it->foodItemId, 0, quantity));
  } while (true);
  return menuItems;
}

void ChefHandler::createTodaysMenu() {
  std::string menuName;
  std::cout << "Enter the menu name: ";
  std::getline(std::cin >> std::ws, menuName);
  DTO::Category category = getCategoryInput();
  DTO::Menu menu{0, menuName, (uint64_t)category, false, std::string("")};
  std::vector<DTO::MenuItem> menuItems = getMenuItemInput();
  Array<DTO::MenuItem> menuArray(menuItems);
  Pair<DTO::Menu, Array<DTO::MenuItem>> menuPair(menu, menuArray);
  std::vector<unsigned char> payload = menuPair.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/createMenu", payload);
  auto response = clientCommunicator.receiveResponse();
  std::cout << "received response, with status : " << response.first << "\n";
  if (response.first == 0) {
    std::cout << "Menu created successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to create menu due to : "
              << (std::string)responseString << "\n";
  }
}

void ChefHandler::RolloutNextMenu() {
  std::string menuName;
  std::cout << "Enter the menu name: ";
  std::getline(std::cin >> std::ws, menuName);
  DTO::Category category = getCategoryInput();

  DTO::Menu menu{0, menuName, (uint64_t)category, true, getDate(24)};
  std::vector<DTO::MenuItem> menuItems = getMenuItemInput();
  Array<DTO::MenuItem> menuArray(menuItems);
  Pair<DTO::Menu, Array<DTO::MenuItem>> menuPair(menu, menuArray);
  std::vector<unsigned char> payload = menuPair.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/createMenu", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Menu created successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to create menu due to : "
              << (std::string)responseString << "\n";
  }
}

void ChefHandler::displayVotingResult(
    const std::vector<DTO::VotingResult> &votingResults) {
  std::cout << "\n.............Voting Results.....................\n";
  if (votingResults.size() == 0) {
    std::cout << "No voting results available\n";
    std::cout << "\n.............End of Voting Results.....................\n";
    return;
  }
  for (auto &voting : votingResults) {
    std::cout << "\n-----------------\n";
    std::cout << "Food item Id : " << voting.foodItemId << "\n";
    std::cout << "Food Item Name : " << (std::string)voting.foodItemName
              << "\n";
    std::cout << "Number Of votes : " << voting.numberOfVotes << "\n";
    std::cout << "Number of positive votes : " << voting.numberOfPositiveVotes
              << "\n";
    std::cout << "Number of negative votes : " << voting.numberOfNegativeVotes
              << "\n";
    std::cout << "\n-----------------\n";
  }
  std::cout << "\n.............End of Voting Results.....................\n";
}

void ChefHandler::viewFeedback() {
  DTO::Category category = getCategoryInput();
  Pair<U64, SString> data((uint64_t)category, getDate(0));
  std::vector<unsigned char> payload = data.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/viewFeedback", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::VotingResult> votingArray;
    votingArray.deserialize(response.second);
    displayVotingResult(votingArray);
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to view feedback due to : "
              << (std::string)responseString << "\n";
  }
}

std::vector<DTO::FoodItem> ChefHandler::getFoodItemsBelowRating(double rating) {
  std::vector<unsigned char> payload;
  Double ratingValue(rating);
  payload = ratingValue.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/getFoodItemsBelowRating", payload);
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

void ChefHandler::showFoodItemsToBeDiscarded() {
  std::vector<DTO::FoodItem> foodItems = getFoodItemsBelowRating(2.0);
  showFoodItems(foodItems);
}

void ChefHandler::discardFoodItem() {
  std::vector<DTO::FoodItem> foodItems = getFoodItemsBelowRating(2.0);
  showFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::cout << "Enter the item id to discard: ";
  InputHandler inputHandler;
  U64 itemId =
      inputHandler.getInput<uint64_t>([itemIdRange](const uint64_t &input) {
        return input >= itemIdRange.first && input <= itemIdRange.second;
      });
  std::vector<unsigned char> payload = itemId.serialize();

  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/discardFoodItem", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Food item discarded successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to discard food item due to : "
              << (std::string)responseString << "\n";
  }
}

std::vector<DTO::DiscardFeedbackQuestion>
ChefHandler::getDiscardFeedbackQuestions(U64 foodItemId) {
  std::vector<unsigned char> payload = foodItemId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/viewDiscardQuestion", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions;
    discardFeedbackQuestions.deserialize(response.second);
    std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestionsVector =
        discardFeedbackQuestions;
    return discardFeedbackQuestionsVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch discard feedback questions, due to : " +
                     (std::string)responseString);
}

std::vector<DTO::FoodItem> ChefHandler::getDiscardedFoodItems() {
  std::vector<unsigned char> payload;
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/getDiscardedFoodItems", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<DTO::FoodItem> foodItems;
    foodItems.deserialize(response.second);
    std::vector<DTO::FoodItem> foodItemsVector = foodItems;
    return foodItemsVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch discarded food items, due to : " +
                     (std::string)responseString);
}

void ChefHandler::displayDiscardFeedbackQuestions(
    const std::vector<DTO::DiscardFeedbackQuestion> &questions) {
  std::cout
      << "\n.............Discard Feedback Questions.....................\n";
  for (auto &discardFeedbackQuestion : questions) {
    std::cout << "\n-----------------\n";
    std::cout << "Question Id : " << discardFeedbackQuestion.questionId << "\n";
    std::cout << "Question : " << (std::string)discardFeedbackQuestion.question
              << "\n";
    std::cout << "\n-----------------\n";
  }
  std::cout << "\n.............End of Discard Feedback "
               "Questions.....................\n";
}

void ChefHandler::showDiscardFeedbackQuestions() {
  std::vector<DTO::FoodItem> foodItems = getDiscardedFoodItems();
  showFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::cout << "Enter the item id to view discard feedback questions: ";
  InputHandler inputHandler;
  U64 itemId =
      inputHandler.getInput<uint64_t>([itemIdRange](const uint64_t &input) {
        return input >= itemIdRange.first && input <= itemIdRange.second;
      });
  std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions =
      getDiscardFeedbackQuestions(itemId);
  displayDiscardFeedbackQuestions(discardFeedbackQuestions);
}

void ChefHandler::addDiscardFeedbackQuestion() {
  std::vector<DTO::FoodItem> foodItems = getDiscardedFoodItems();
  showFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::cout << "Enter the item id to add discard feedback question: ";
  InputHandler inputHandler;
  U64 itemId =
      inputHandler.getInput<uint64_t>([itemIdRange](const uint64_t &input) {
        return input >= itemIdRange.first && input <= itemIdRange.second;
      });
  std::string question;
  std::cout << "Enter the question: ";
  std::getline(std::cin >> std::ws, question);
  Pair<SString, U64> discardFeedbackQuestion(question, itemId);
  std::vector<unsigned char> payload = discardFeedbackQuestion.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/addDiscardQuestion", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Discard feedback question added successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to add discard feedback question due to : "
              << (std::string)responseString << "\n";
  }
}
