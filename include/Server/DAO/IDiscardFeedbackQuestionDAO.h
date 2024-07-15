#pragma once

#include "DiscardFeedbackQuestion.h"
#include <cstdint>
namespace DAO {

class IDiscardFeedbackQuestionDAO {
public:
  virtual bool addQuestion(DTO::DiscardFeedbackQuestion) = 0;
  virtual bool deleteQuestion(uint64_t questionId) = 0;
  virtual bool updateQuestion(DTO::DiscardFeedbackQuestion) = 0;
  virtual DTO::DiscardFeedbackQuestion getQuestionById(uint64_t questionId) = 0;
  virtual std::vector<DTO::DiscardFeedbackQuestion>
  getQuestionsByFoodItemId(uint64_t foodItemId) = 0;
  virtual std::vector<DTO::DiscardFeedbackQuestion> getAllQuestions() = 0;
  virtual ~IDiscardFeedbackQuestionDAO() = default;
};
}; // namespace DAO