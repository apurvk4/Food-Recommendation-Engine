#pragma once

#include "DAO/IFoodItemDAO.h"
#include "DAO/INotificationDAO.h"
#include "DTO/FoodItem.h"
#include "UserService.h"
#include <cstdint>

namespace Service {

using DAO::IFoodItemDAO;
using DAO::INotificationDAO;
using DTO::FoodItem;
using DTO::User;

class AdminService : public UserService {
  std::shared_ptr<IFoodItemDAO> foodItemDAO;

public:
  AdminService(std::shared_ptr<IUserDAO> userDAO,
               std::shared_ptr<IRoleDAO> roleDAO,
               std::shared_ptr<INotificationDAO> notificationDAO,
               std::shared_ptr<IFoodItemDAO> foodItemDAO);
  ~AdminService();
  bool addUser(uint64_t adminUserId, User newUser);
  bool addFoodItem(uint64_t adminUserId, FoodItem foodItem);
  bool removeFoodItem(uint64_t adminUserId, uint64_t foodItemId);
  bool updateFoodItem(uint64_t adminUserId, FoodItem foodItem);
};

}; // namespace Service