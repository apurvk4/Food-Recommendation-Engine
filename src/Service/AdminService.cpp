#include "Service/AdminService.h"
#include <stdexcept>

using Service::AdminService;

AdminService::AdminService(std::shared_ptr<IUserDAO> userDAO,
                           std::shared_ptr<IRoleDAO> roleDAO,
                           std::shared_ptr<INotificationDAO> notificationDAO,
                           std::shared_ptr<IFoodItemDAO> foodItemDAO)
    : UserService(userDAO, roleDAO, notificationDAO), foodItemDAO(foodItemDAO) {
}

AdminService::~AdminService() {}

bool AdminService::addUser(uint64_t adminUserId, User newUser) {
  User adminUser = getUserById(adminUserId);
  if (adminUser.roleId != (U64)1) {
    throw std::runtime_error("Only admin can add new user");
  }
  return userDAO->addUser(newUser);
}

bool AdminService::addFoodItem(uint64_t adminUserId, FoodItem foodItem) {
  User adminUser = getUserById(adminUserId);
  if (adminUser.roleId != (U64)1) {
    throw std::runtime_error("Only admin can add new food item");
  }
  return foodItemDAO->addFoodItem(foodItem);
}

bool AdminService::removeFoodItem(uint64_t adminUserId, uint64_t foodItemId) {
  User adminUser = getUserById(adminUserId);
  if (adminUser.roleId != (U64)1) {
    throw std::runtime_error("Only admin can remove food item");
  }
  return foodItemDAO->deleteFoodItem(foodItemId);
}

bool AdminService::updateFoodItem(uint64_t adminUserId, FoodItem foodItem) {
  User adminUser = getUserById(adminUserId);
  if (adminUser.roleId != (U64)1) {
    throw std::runtime_error("Only admin can update food item");
  }
  return foodItemDAO->updateFoodItem(foodItem);
}

std::vector<DTO::FoodItem> AdminService::getFoodItems(uint64_t adminUserId) {
  User adminUser = getUserById(adminUserId);
  if (adminUser.roleId != (U64)1) {
    throw std::runtime_error("Only admin can update food item");
  }
  std::cout << "admin verified \n";
  if (foodItemDAO != nullptr) {
    auto result = foodItemDAO->getAllFoodItems();
    std::cout << "found alll food Items\n";
    return result;
  } else {
    std::cout << "foodItem DAO is null\n";
    throw std::runtime_error("foodItemDAO is null");
  }
}