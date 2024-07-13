#include "Client/ChefHandler.h"
#include "Category.h"
#include "Client/ClientCommunicator.h"
#include "Client/InputHandler.h"
#include "FoodItem.h"
#include "Menu.h"
#include "MenuItem.h"
#include "Role.h"
#include "SerializableTypes/Pair.h"
#include "Util.h"
#include "VotingResult.h"
#include <algorithm>
#include <cstdint>
#include <string>

void ChefHandler::performAction() {
  if (isLoggedIn || !login()) {
    return;
  }
  int choice;
  InputHandler inputHandler;
  do {
    std::cout << "\n\n1. Show Food Items\n"
              << "2. Show Recommended Food Items\n"
              << "3. create Menu\n"
              << "4. rollout next menu\n"
              << "5. update Menu \n"
              << "6. view Feedback\n"
              << "7. view Notifications\n"
              << "8. Get Discarded Food Items\n"
              << "9. Discard Food Item\n"
              << "10. Show Discard Feedback Questions\n"
              << "11. Add Discard Feedback Question\n"
              << "12. Show Discard Feedback Answers \n"
              << "13. Logout\n"
              << "Enter your choice: ";
    choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 13; });
    std::cout << "\n\n";
    handleUserSelection(choice);
  } while (choice != 13);
}

void ChefHandler::handleUserSelection(int choice) {
  switch (choice) {
  case 1:
    showFoodItems();
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
    updateMenu();
    break;
  case 6:
    viewFeedback();
    break;
  case 7:
    viewNotifications("/Chef/viewNotifications");
    break;
  case 8:
    displayFoodItems(getDiscardedFoodItems());
    break;
  case 9:
    discardFoodItem();
    break;
  case 10:
    showDiscardFeedbackQuestions();
    break;
  case 11:
    addDiscardFeedbackQuestion();
  case 12:
    viewDiscardFeedbackAnswers();
    break;
  case 13:
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

void ChefHandler::updateMenu() {
  std::cout << "Enter 1 to update menu or 2 to update rollout menu: ";
  InputHandler inputHandler;
  int choice = inputHandler.getInput<int>(
      [](const int &input) { return input == 1 || input == 2; });
  std::cout << "Enter the date for which you want to update the menu \n";
  std::string date = getDateInput();
  if (choice == 1) {
    updateMenu("/Chef/viewMenu", date);
  } else {
    updateMenu("/Chef/viewRolloutMenu", date);
  }
}

std::vector<DTO::MenuItem>
ChefHandler::getUpdatedMenuItems(std::vector<DTO::MenuItem> &menuItems) {
  std::pair<uint64_t, uint64_t> itemIdRange = getMenuItemIdRange(menuItems);
  std::vector<DTO::MenuItem> updatedMenuItems;
  InputHandler inputHandler;
  std::cout << "Enter the item ids to keep in Menu. Enter 0 to stop\n";
  do {
    std::cout << "Enter item id: ";
    uint64_t itemId =
        inputHandler.getInput<uint64_t>([&itemIdRange](const uint64_t &input) {
          return input >= itemIdRange.first && input <= itemIdRange.second;
        });
    if (itemId == 0) {
      break;
    }
    auto it = std::find_if(menuItems.begin(), menuItems.end(),
                           [itemId](const DTO::MenuItem &menuItem) {
                             return (uint64_t)menuItem.menuItemId == itemId;
                           });
    std::cout << "Enter quantity : ";
    uint64_t quantity = inputHandler.getInput<uint64_t>(
        [](const uint64_t &input) { return input > 0; });
    updatedMenuItems.push_back(
        DTO::MenuItem(it->menuItemId, it->foodItemId, it->menuId, quantity));
  } while (true);
  return updatedMenuItems;
}

void ChefHandler::displayMenuItems(
    const std::vector<DTO::MenuItem> &menuItems) {
  std::cout << "\n....................Menu Items....................\n";
  if (menuItems.size() == 0) {
    std::cout << "No menu items available\n";
    std::cout
        << "\n....................End of Menu Items....................\n";
    return;
  }
  for (auto &menuItem : menuItems) {
    std::cout << "Menu Item Id : " << menuItem.menuItemId << "\n";
    std::cout << "Food Item Id : " << menuItem.foodItemId << "\n";
    std::cout << "Menu Id : " << menuItem.menuId << "\n";
    std::cout << "Quantity : " << menuItem.quantity << "\n";
    std::cout << "\n-----------------\n";
  }
  std::cout << "\n....................End of Menu Items....................\n";
}

std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
ChefHandler::getMenu(std::string endpoint, SString date) {
  std::vector<unsigned char> payload = date.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId, endpoint,
                                 payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<DTO::Menu, Array<DTO::MenuItem>>> menus;
    menus.deserialize(response.second);
    std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>> menusVector;
    for (int i = 0; i < menus.NumberOfItems(); i++) {
      DTO::Menu menu = menus[i].first;
      Array<DTO::MenuItem> menuItems = menus[i].second;
      std::vector<DTO::MenuItem> menuItemsVector;
      for (int j = 0; j < menuItems.NumberOfItems(); j++) {
        menuItemsVector.push_back(menuItems[j]);
      }
      menusVector.push_back({menu, menuItemsVector});
    }
    return menusVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch menus, due to : " +
                     (std::string)responseString);
}

void ChefHandler::updateMenu(std::string endpoint, std::string date) {
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>> menus =
      getMenu(endpoint, date);
  if (menus.size() == 0) {
    std::cout << "No menus available\n";
    return;
  }
  uint64_t minMenuId = menus[0].first.menuId;
  uint64_t maxMenuId = menus[0].first.menuId;
  for (int i = 0; i < menus.size(); i++) {
    minMenuId = std::min(minMenuId, (uint64_t)menus[i].first.menuId);
    maxMenuId = std::max(maxMenuId, (uint64_t)menus[i].first.menuId);
    displayMenu(menus[i].first);
  }
  std::cout << "Select the menuId to update\n";
  InputHandler inputHandler;
  int choice =
      inputHandler.getInput<int>([minMenuId, maxMenuId](const int &input) {
        return input >= minMenuId && input <= maxMenuId;
      });
  auto it = std::find_if(
      menus.begin(), menus.end(),
      [choice](const std::pair<DTO::Menu, std::vector<DTO::MenuItem>> &menu) {
        return (uint64_t)menu.first.menuId == choice;
      });
  DTO::Menu &menu = it->first;
  std::string menuName;
  std::cout << "Enter the new menu name: ";
  std::getline(std::cin >> std::ws, menuName);
  menu.menuName = menuName;
  displayMenuItems(it->second);
  std::vector<DTO::MenuItem> menuItems = getUpdatedMenuItems(it->second);
  Array<DTO::MenuItem> menuArray(menuItems);
  Pair<DTO::Menu, Array<DTO::MenuItem>> menuPair(menu, menuArray);
  std::vector<unsigned char> payload = menuPair.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/updateMenu", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    std::cout << "Menu updated successfully\n";
  } else {
    SString responseString;
    responseString.deserialize(response.second);
    std::cout << "Failed to update menu due to : "
              << (std::string)responseString << "\n";
  }
}

void ChefHandler::showFoodItems() {
  std::vector<DTO::FoodItem> foodItems = getFoodItems();
  displayFoodItems(foodItems);
}

void ChefHandler::displayMenu(const DTO::Menu &menu) {
  std::cout << "\n....................Menu....................\n";
  std::cout << "Menu Id : " << menu.menuId << "\n";
  std::cout << "Menu Name : " << (std::string)menu.menuName << "\n";
  std::cout << "Category : "
            << DTO::CategoryToString((DTO::Category)(uint64_t)menu.categoryId)
            << "\n";
  std::cout << "Is Rollout : " << (menu.isSurvey ? "Yes" : "No") << "\n";
  std::cout << "Date : " << (std::string)menu.date << "\n";
  std::cout << "\n....................End of Menu....................\n";
}

std::vector<DTO::FoodItem> ChefHandler::getFoodItems(DTO::Category category) {
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

std::vector<DTO::FoodItem> ChefHandler::getFoodItems() {
  DTO::Category category = getCategoryInput();
  return getFoodItems(category);
}

void ChefHandler::showFoodItemRecommendation() {
  std::vector<DTO::FoodItem> foodItems = getFoodItemRecommendation();
  displayFoodItems(foodItems);
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
ChefHandler::getMenuItemIdRange(std::vector<DTO::MenuItem> &menuItems) {
  if (menuItems.size() <= 0) {
    return std::make_pair(0, 0);
  }
  std::pair<uint64_t, uint64_t> range =
      std::make_pair(menuItems[0].menuItemId, menuItems[0].menuItemId);
  for (auto &menuItem : menuItems) {
    range.first = std::min(range.first, (uint64_t)menuItem.menuItemId);
    range.second = std::max(range.second, (uint64_t)menuItem.menuItemId);
  }
  return range;
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

std::vector<DTO::MenuItem>
ChefHandler::getMenuItemInput(DTO::Category category) {
  std::vector<DTO::FoodItem> foodItems = getFoodItems(category);
  displayFoodItems(foodItems);
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
  std::vector<DTO::MenuItem> menuItems = getMenuItemInput(category);
  if (menuItems.size() == 0) {
    std::cout << "No menu items added. Exiting\n";
    return;
  }
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
  std::vector<DTO::MenuItem> menuItems = getMenuItemInput(category);
  if (menuItems.size() == 0) {
    std::cout << "No menu items added. Exiting\n";
    return;
  }
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
  Pair<U64, SString> data((uint64_t)category, getDate(24));
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
  displayFoodItems(foodItems);
}

void ChefHandler::discardFoodItem() {
  double thresholdRating = 0.0;
  // ask user to enter the threshold rating
  std::cout << "Enter the threshold rating between (0.0 to 5.0): ";
  InputHandler inputHandler;
  thresholdRating = inputHandler.getInput<double>(
      [](const double &input) { return input >= 0.0 && input <= 5.0; });
  std::vector<DTO::FoodItem> foodItems =
      getFoodItemsBelowRating(thresholdRating);
  displayFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::cout << "Enter the item id to discard: ";
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
  displayFoodItems(foodItems);
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
  displayFoodItems(foodItems);
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

std::vector<std::pair<double, std::vector<std::string>>>
ChefHandler::getAnswerSentiments(U64 questionId) {
  std::vector<unsigned char> payload = questionId.serialize();
  ClientCommunicator clientCommunicator(SERVER_IP, SERVER_PORT);
  clientCommunicator.sendRequest(user.userId, (uint64_t)roleId,
                                 "/Chef/getAnswerSentiments", payload);
  auto response = clientCommunicator.receiveResponse();
  if (response.first == 0) {
    Array<Pair<Double, Array<SString>>> sentimentScores;
    sentimentScores.deserialize(response.second);
    std::vector<std::pair<double, std::vector<std::string>>>
        sentimentScoresVector;
    for (int i = 0; i < sentimentScores.NumberOfItems(); i++) {
      double sentimentScore = sentimentScores[i].first;
      Array<SString> words = sentimentScores[i].second;
      std::vector<std::string> wordsVector;
      for (int j = 0; j < words.NumberOfItems(); j++) {
        wordsVector.push_back((std::string)words[j]);
      }
      sentimentScoresVector.push_back({sentimentScore, wordsVector});
    }
    return sentimentScoresVector;
  }
  SString responseString;
  responseString.deserialize(response.second);
  std::runtime_error("Failed to fetch sentiment scores, due to : " +
                     (std::string)responseString);
}

void ChefHandler::showDiscardFeedbackAnswers(
    const std::vector<std::pair<double, std::vector<std::string>>>
        &answerSentiments) {
  std::cout << "\n.............Discard Feedback Answers.....................\n";
  for (auto &answerSentiment : answerSentiments) {
    std::cout << "\n-----------------\n";
    std::cout << "Sentiment Score : " << answerSentiment.first << "\n";
    std::cout << "Frequent Words : \n";
    for (auto &word : answerSentiment.second) {
      std::cout << word << " ";
    }
    std::cout << "\n-----------------\n";
  }
  std::cout << "\n.............End of Discard Feedback "
               "Answers.....................\n";
}

void ChefHandler::viewDiscardFeedbackAnswers() {
  std::vector<DTO::FoodItem> foodItems = getDiscardedFoodItems();
  displayFoodItems(foodItems);
  std::pair<uint64_t, uint64_t> itemIdRange = getItemIdRange(foodItems);
  std::cout << "Enter the item id to view discard feedback answers: ";
  InputHandler inputHandler;
  U64 itemId =
      inputHandler.getInput<uint64_t>([itemIdRange](const uint64_t &input) {
        return input >= itemIdRange.first && input <= itemIdRange.second;
      });
  std::vector<DTO::DiscardFeedbackQuestion> discardFeedbackQuestions =
      getDiscardFeedbackQuestions(itemId);
  displayDiscardFeedbackQuestions(discardFeedbackQuestions);
  std::cout << "Enter the question id to view answers: ";
  U64 questionId = inputHandler.getInput<uint64_t>(
      [discardFeedbackQuestions](const uint64_t &input) {
        for (auto &discardFeedbackQuestion : discardFeedbackQuestions) {
          if ((uint64_t)discardFeedbackQuestion.questionId == input) {
            return true;
          }
        }
        return false;
      });
  std::vector<std::pair<double, std::vector<std::string>>> answerSentiments =
      getAnswerSentiments(questionId);
  showDiscardFeedbackAnswers(answerSentiments);
}