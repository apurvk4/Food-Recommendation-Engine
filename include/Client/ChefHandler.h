#pragma once

#include "Category.h"
#include "Feedback.h"
#include "FoodItem.h"
#include "Menu.h"
#include "UserHandler.h"
#include "VotingResult.h"
#include <cstdint>

class ChefHandler : public UserHandler {
  bool isLoggedIn;

  void handleUserSelection(int choice);
  std::vector<DTO::FoodItem> getFoodItems();
  void showFoodItems(const std::vector<DTO::FoodItem> &foodItems);
  std::vector<DTO::FoodItem> getFoodItemRecommendation();
  void showFoodItemRecommendation();
  void createTodaysMenu();
  void RolloutNextMenu();
  std::vector<DTO::Feedback> getFeedback();
  void viewFeedback();
  std::pair<uint64_t, uint64_t>
  getItemIdRange(std::vector<DTO::FoodItem> &foodItems);
  std::vector<DTO::MenuItem> getMenuItemInput();
  void displayVotingResult(const std::vector<DTO::VotingResult> &votingResults);

public:
  ChefHandler();
  void performAction() override;
  bool login() override;
  bool logout() override;
  ~ChefHandler() = default;
};