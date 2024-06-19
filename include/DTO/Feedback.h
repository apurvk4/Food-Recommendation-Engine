#pragma once

#include <cstdint>
#include <string>

namespace DTO {

struct Feedback {
  uint64_t feedbackId;
  uint64_t userId;
  uint64_t menuId;
  uint64_t foodItemId;
  uint32_t rating;
  std::string comment;
  std::string date;
  Feedback(uint64_t feedbackId, uint64_t userId, uint64_t menuId,
           uint64_t foodItemId, uint32_t rating, std::string comment,
           std::string date)
      : feedbackId(feedbackId), userId(userId), menuId(menuId),
        foodItemId(foodItemId), rating(rating), comment(comment), date(date) {}
};

}; // namespace DTO