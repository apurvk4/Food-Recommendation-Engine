#pragma once

#include "DbConnection.h"
#include "IUserResponseDAO.h"
class UserResponseDAO : public IUserResponseDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  UserResponseDAO();
  bool saveUserResponse(UserResponse) override;
  UserResponse getUserResponse(uint64_t responseId) override;
  std::vector<UserResponse>
  getUserResponseForAllFoodItemsInMenu(uint64_t menuId) override;
  std::vector<UserResponse>
  getUserResponseForFoodItemInMenu(uint64_t foodItemId,
                                   uint64_t menuId) override;
  ~UserResponseDAO() = default;
};