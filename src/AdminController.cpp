#include "AdminController.h"
#include "FoodItemDAO.h"
#include "NotificationDAO.h"
#include "RoleDAO.h"
#include "UserDAO.h"
#include <memory>

using Controller::AdminController;

AdminController::AdminController()
    : adminService(std::make_shared<DAO::UserDAO>(),
                   std::make_shared<DAO::RoleDAO>(),
                   std::make_shared<DAO::NotificationDAO>(),
                   std::make_shared<DAO::FoodItemDAO>()) {}

AdminController::~AdminController() {}

bool AdminController::addUser(uint64_t adminUserId, DTO::User newUser) {
  return adminService.addUser(adminUserId, newUser);
}

bool AdminController::addFoodItem(uint64_t adminUserId,
                                  DTO::FoodItem foodItem) {
  return adminService.addFoodItem(adminUserId, foodItem);
}

bool AdminController::removeFoodItem(uint64_t adminUserId,
                                     uint64_t foodItemId) {
  return adminService.removeFoodItem(adminUserId, foodItemId);
}

bool AdminController::updateFoodItem(uint64_t adminUserId,
                                     DTO::FoodItem foodItem) {
  return adminService.updateFoodItem(adminUserId, foodItem);
}

Array<DTO::FoodItem> AdminController::getFoodItems(uint64_t adminUserId) {
  auto result = adminService.getFoodItems(adminUserId);
  Array<DTO::FoodItem> resultArray;
  for (auto item : result) {
    resultArray.push_back(item);
  }
  return resultArray;
}