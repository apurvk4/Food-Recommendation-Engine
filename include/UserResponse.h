#pragma once

#include <cstdint>
struct UserResponse {
  uint64_t responseId;
  uint64_t userId;
  uint64_t menuId;
  uint64_t foodItemId;
  bool preference;
  UserResponse(uint64_t responseId, uint64_t userId, uint64_t menuId,
               uint64_t foodItemId, bool preference)
      : responseId(responseId), userId(userId), menuId(menuId),
        foodItemId(foodItemId), preference(preference) {}
};