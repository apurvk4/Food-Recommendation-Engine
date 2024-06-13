#pragma once

#include "Feedback.h"
#include <vector>

class IFeedbackDAO {
public:
  virtual bool addFeedback(Feedback) = 0;
  virtual bool deleteFeedback(uint64_t feedbackId) = 0;
  virtual bool updateFeedback(Feedback) = 0;
  virtual Feedback getFeedbackById(uint64_t feedbackId) = 0;
  virtual std::vector<Feedback> getFeedbacksByMenuId(uint64_t menuId) = 0;
  virtual std::vector<Feedback> getFeedbacksByUserId(uint64_t userId) = 0;
  virtual std::vector<Feedback> getAllFeedbacks() = 0;
  virtual ~IFeedbackDAO() = default;
};