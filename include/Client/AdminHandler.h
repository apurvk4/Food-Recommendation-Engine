#pragma once

#include "Category.h"
#include "FoodItem.h"
#include "Notification.h"
#include "UserHandler.h"
#include <cstdint>

class AdminHandler : public UserHandler {
  bool isLoggedIn;

  void handleUserSelection(int choice);
  void showFoodItems();
  std::vector<DTO::FoodItem> getFoodItems();
  void displayFoodItems(std::vector<DTO::FoodItem> &foodItems);
  void addFoodItem();
  void updateFoodItem();
  void deleteFoodItem();
  void setFoodItemAttribute();
  void getFoodItemAttribute();
  void removeFoodItemAttribute();
  std::vector<std::pair<uint64_t, std::string>> getAllAttributes();
  bool updateFoodItemAttribute(U64 foodItemId, U64 attributeId,
                               const std::string &endpoint);
  void addAttribute();
  void addAttribte(const std::string &attributeName);

public:
  AdminHandler();
  void performAction() override;
  bool login() override;
  bool logout() override;
  ~AdminHandler() = default;
};