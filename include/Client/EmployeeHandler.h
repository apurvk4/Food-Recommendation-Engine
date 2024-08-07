#pragma once

#include "DiscardFeedbackQuestion.h"
#include "Menu.h"
#include "UserHandler.h"
#include <cstdint>

class EmployeeHandler : public UserHandler {
  bool isLoggedIn;

  std::pair<DTO::Menu, std::vector<std::pair<DTO::FoodItem, double>>>
  getMenu(U64 categoryId, SString date);
  std::pair<DTO::Menu, std::vector<std::pair<double, DTO::FoodItem>>>
  getRolloutMenu(U64 categoryId, SString date);
  std::vector<std::pair<uint64_t, std::string>> getAllAttributes();
  std::vector<std::pair<uint64_t, std::string>> getFoodItemPreferences();
  void handleUserSelection(int choice);
  void showTodaysMenu();
  void showFoodItemPreferences();
  void deleteFoodItemPreference();
  void setFoodItemPreference();
  void showNextMenuRollout();
  void provideRolloutFeedback();
  void addReview();
  void displayMenu(
      const std::pair<DTO::Menu, std::vector<std::pair<DTO::FoodItem, double>>>
          &menu);
  void
  displayMenu(const DTO::Menu menu,
              std::vector<std::pair<double, DTO::FoodItem>> foodItemWithRating);
  std::vector<DTO::FoodItem> getDiscardedFoodItems();
  void showDiscardedFoodItems();
  std::vector<DTO::DiscardFeedbackQuestion>
  getDiscardFeedbackQuestions(U64 foodItemId);
  void showDiscardFeedbackQuestions();
  void displayDiscardFeedbackQuestions(
      const std::vector<DTO::DiscardFeedbackQuestion> &questions);
  void addDiscardFeedbackAnswer();

public:
  EmployeeHandler();
  void performAction() override;
  bool login() override;
  bool logout() override;
  ~EmployeeHandler() = default;
};