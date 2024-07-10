#pragma once

#include "DTO/DiscardFeedbackAnswer.h"

namespace DAO {
class IDiscardFeedbackAnswerDAO {
public:
  virtual bool addAnswer(DTO::DiscardFeedbackAnswer) = 0;
  virtual bool deleteAnswer(uint64_t answerId) = 0;
  virtual bool updateAnswer(DTO::DiscardFeedbackAnswer) = 0;
  virtual DTO::DiscardFeedbackAnswer getAnswerById(uint64_t answerId) = 0;
  virtual std::vector<DTO::DiscardFeedbackAnswer>
  getAnswersByQuestionId(uint64_t questionId) = 0;
  virtual std::vector<DTO::DiscardFeedbackAnswer> getAllAnswers() = 0;
  virtual ~IDiscardFeedbackAnswerDAO() = default;
};
}; // namespace DAO