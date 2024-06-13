#pragma once

#include "DbConnection.h"
#include "IFeedbackDAO.h"
#include <memory>

class FeedbackDAO : public IFeedbackDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FeedbackDAO();
  bool addFeedback(Feedback) override;
  bool deleteFeedback(uint64_t feedbackId) override;
  bool updateFeedback(Feedback) override;
  Feedback getFeedbackById(uint64_t feedbackId) override;
  std::vector<Feedback> getFeedbacksByMenuId(uint64_t menuId) override;
  std::vector<Feedback> getFeedbacksByUserId(uint64_t userId) override;
  std::vector<Feedback> getAllFeedbacks() override;
};