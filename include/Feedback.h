#pragma once

#include <cstdint>
#include <string>

struct Feedback {
  uint64_t feedbackId;
  uint64_t userId;
  uint64_t menuId;
  std::string comment;
  int rating;
  std::string date;
  Feedback(uint64_t feedbackId, uint64_t userId, uint64_t menuId,
           std::string comment, int rating, std::string date)
      : feedbackId{feedbackId}, userId{userId}, menuId{menuId},
        comment{comment}, rating{rating}, date{date} {}
};