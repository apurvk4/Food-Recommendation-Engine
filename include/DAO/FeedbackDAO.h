#pragma once

#include "DbConnection.h"
#include "IFeedbackDAO.h"
#include <memory>

namespace DAO {

using DAO::IFeedbackDAO;
using DTO::Feedback;

class FeedbackDAO : public IFeedbackDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FeedbackDAO();
  bool addFeedback(Feedback) override;
  Feedback getFeedbackById(uint64_t feedbackId) override;
  std::vector<Feedback> getFeedbacksByMenuId(uint64_t menuId) override;
  std::vector<Feedback> getFeedbacksByUserId(uint64_t userId) override;
  std::vector<Feedback> getFeedbacksByFoodItemId(uint64_t foodItemId) override;
  std::vector<Feedback> getAllFeedbacks() override;
};

}; // namespace DAO