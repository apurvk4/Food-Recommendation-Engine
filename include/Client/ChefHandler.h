#pragma once

#include "Category.h"
#include "DiscardFeedbackQuestion.h"
#include "Feedback.h"
#include "FoodItem.h"
#include "Menu.h"
#include "MenuItem.h"
#include "UserHandler.h"
#include "VotingResult.h"
#include <cstdint>

class ChefHandler : public UserHandler {
  bool isLoggedIn;
  void handleUserSelection(int choice);
  void showFoodItems();
  std::vector<DTO::FoodItem> getFoodItems(DTO::Category category);
  std::vector<DTO::FoodItem> getFoodItems();
  std::vector<DTO::FoodItem> getFoodItemRecommendation();
  void showFoodItemRecommendation();
  void createTodaysMenu();
  void RolloutNextMenu();
  void updateMenu();
  void updateMenu(std::string endpoint, std::string date);
  std::vector<DTO::MenuItem>
  getUpdatedMenuItems(std::vector<DTO::MenuItem> &menuItems);
  void displayMenu(const DTO::Menu &menu);
  std::vector<std::pair<DTO::Menu, std::vector<DTO::MenuItem>>>
  getMenu(std::string endpoint, SString date);
  std::vector<DTO::Feedback> getFeedback();
  void viewFeedback();
  std::pair<uint64_t, uint64_t>
  getItemIdRange(std::vector<DTO::FoodItem> &foodItems);
  std::pair<uint64_t, uint64_t>
  getMenuItemIdRange(std::vector<DTO::MenuItem> &menuItems);
  void displayMenuItems(const std::vector<DTO::MenuItem> &menuItems);
  std::vector<DTO::MenuItem> getMenuItemInput(DTO::Category category);
  void displayVotingResult(const std::vector<DTO::VotingResult> &votingResults);
  std::vector<DTO::FoodItem> getFoodItemsBelowRating(double rating);
  void showFoodItemsToBeDiscarded();
  void discardFoodItem();
  std::vector<DTO::DiscardFeedbackQuestion>
  getDiscardFeedbackQuestions(U64 foodItemId);
  void showDiscardFeedbackQuestions();
  void addDiscardFeedbackQuestion();
  void displayDiscardFeedbackQuestions(
      const std::vector<DTO::DiscardFeedbackQuestion> &questions);
  std::vector<DTO::FoodItem> getDiscardedFoodItems();
  std::vector<std::pair<double, std::vector<std::string>>>
  getAnswerSentiments(U64 questionId);
  void showDiscardFeedbackAnswers(
      const std::vector<std::pair<double, std::vector<std::string>>>
          &answerSentiments);
  void viewDiscardFeedbackAnswers();

public:
  ChefHandler();
  void performAction() override;
  bool login() override;
  bool logout() override;
  ~ChefHandler() = default;
};