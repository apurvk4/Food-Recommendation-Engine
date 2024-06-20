#pragma once

#include "AdminService.h"
#include "DAO/IFoodItemDAO.h"
#include "DAO/INotificationDAO.h"
#include "DTO/FoodItem.h"
#include "SerializableTypes/Array.h"
#include "UserService.h"
#include <cstdint>

namespace Controller {
class AdminController {
  Service::AdminService adminService;

public:
  AdminController();
  ~AdminController();
  bool addUser(uint64_t adminUserId, DTO::User newUser);
  bool addFoodItem(uint64_t adminUserId, DTO::FoodItem foodItem);
  bool removeFoodItem(uint64_t adminUserId, uint64_t foodItemId);
  bool updateFoodItem(uint64_t adminUserId, DTO::FoodItem foodItem);
  Array<DTO::FoodItem> getFoodItems(uint64_t adminUserId);
};
}; // namespace Controller