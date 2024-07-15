#pragma once

#include "DTO/DiscardFeedbackQuestion.h"
#include "IDiscardFeedbackQuestionDAO.h"
#include "Server/DbConnection.h"

namespace DAO {

class DiscardFeedbackQuestionDAO : public IDiscardFeedbackQuestionDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  DiscardFeedbackQuestionDAO();
  bool addQuestion(DTO::DiscardFeedbackQuestion) override;
  bool deleteQuestion(uint64_t questionId) override;
  bool updateQuestion(DTO::DiscardFeedbackQuestion) override;
  DTO::DiscardFeedbackQuestion getQuestionById(uint64_t questionId) override;
  std::vector<DTO::DiscardFeedbackQuestion>
  getQuestionsByFoodItemId(uint64_t foodItemId) override;
  std::vector<DTO::DiscardFeedbackQuestion> getAllQuestions() override;
};

}; // namespace DAO