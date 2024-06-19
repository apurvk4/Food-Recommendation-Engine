#pragma once

#include "DTO/Feedback.h"
#include <vector>

namespace DAO {

class IFeedbackDAO {
public:
  virtual bool addFeedback(DTO::Feedback) = 0;
  virtual DTO::Feedback getFeedbackById(uint64_t feedbackId) = 0;
  virtual std::vector<DTO::Feedback> getFeedbacksByMenuId(uint64_t menuId) = 0;
  virtual std::vector<DTO::Feedback> getFeedbacksByUserId(uint64_t userId) = 0;
  virtual std::vector<DTO::Feedback>
  getFeedbacksByFoodItemId(uint64_t foodItemId) = 0;
  virtual std::vector<DTO::Feedback> getAllFeedbacks() = 0;
  virtual ~IFeedbackDAO() = default;
};

}; // namespace DAO