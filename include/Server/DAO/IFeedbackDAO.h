#pragma once

#include "Feedback.h"
#include <cstdint>
namespace DAO {

using DTO::Feedback;

class IFeedbackDAO {
public:
  virtual bool addFeedback(Feedback feedback) = 0;
  virtual bool deleteFeedback(uint64_t feedbackId) = 0;
  virtual bool updateFeedback(DTO::Feedback feedback) = 0;
  virtual std::vector<Feedback> getFeedbackByUserId(uint64_t userId) = 0;
  virtual std::vector<Feedback> getFeedbackByUserId(uint64_t userId,
                                                    std::string date) = 0;
  virtual std::vector<Feedback>
  getFeedbackByFoodItemId(uint64_t foodItemId) = 0;
  virtual std::vector<Feedback> getFeedbackByFoodItemId(uint64_t foodItemId,
                                                        std::string date) = 0;
  virtual ~IFeedbackDAO() = default;
};

}; // namespace DAO