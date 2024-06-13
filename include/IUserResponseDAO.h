#pragma once

#include "UserResponse.h"
#include <cstdint>
#include <vector>

class IUserResponseDAO {
public:
  virtual bool saveUserResponse(UserResponse) = 0;
  virtual UserResponse getUserResponse(uint64_t responseId) = 0;
  virtual std::vector<UserResponse>
  getUserResponseForAllFoodItemsInMenu(uint64_t menuId) = 0;
  virtual std::vector<UserResponse>
  getUserResponseForFoodItemInMenu(uint64_t foodItemId, uint64_t menuId) = 0;
  virtual ~IUserResponseDAO() = default;
};