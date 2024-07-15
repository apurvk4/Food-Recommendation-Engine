#pragma once

#include "Feedback.h"
#include "IFeedbackDAO.h"
#include "Server/DbConnection.h"

namespace DAO {

class FeedbackDAO : public IFeedbackDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  FeedbackDAO();
  bool addFeedback(DTO::Feedback feedback) override;
  bool deleteFeedback(uint64_t feedbackId) override;
  bool updateFeedback(DTO::Feedback feedback) override;
  std::vector<DTO::Feedback> getFeedbackByUserId(uint64_t userId) override;
  std::vector<DTO::Feedback> getFeedbackByUserId(uint64_t userId,
                                                 std::string date) override;
  std::vector<DTO::Feedback>
  getFeedbackByFoodItemId(uint64_t foodItemId) override;
  std::vector<DTO::Feedback> getFeedbackByFoodItemId(uint64_t foodItemId,
                                                     std::string date) override;
  ~FeedbackDAO() = default;
};

}; // namespace DAO