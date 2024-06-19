#pragma once

#include <cstdint>
#include <string>
namespace DTO {

struct Preference {
  uint64_t preferenceId;
  uint64_t userId;
  uint64_t foodItemId;
  uint64_t menuId;
  bool preference;
  std::string date;
  Preference(uint64_t preferenceId, uint64_t userId, uint64_t foodItemId,
             uint64_t menuId, bool preference, std::string date)
      : preferenceId(preferenceId), userId(userId), foodItemId(foodItemId),
        menuId(menuId), preference(preference), date(date) {}
};

}; // namespace DTO