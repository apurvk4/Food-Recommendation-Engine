#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>

namespace DTO {
struct DiscardFeedbackAnswer : public Serializable {
  U64 answerId;
  U64 questionId;
  U64 userId;
  SString answer;
  DiscardFeedbackAnswer() : answerId(0), questionId(0), userId(0), answer("") {}
  DiscardFeedbackAnswer(U64 answerId, U64 questionId, U64 userId,
                        SString answer)
      : answerId(answerId), questionId(questionId), userId(userId),
        answer(answer) {}
  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> answerIdSerialized = answerId.serialize();
    std::vector<unsigned char> questionIdSerialized = questionId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> answerSerialized = answer.serialize();
    serialized.insert(serialized.end(), answerIdSerialized.begin(),
                      answerIdSerialized.end());
    serialized.insert(serialized.end(), questionIdSerialized.begin(),
                      questionIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), answerSerialized.begin(),
                      answerSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) {
    uint64_t bytesRead = 0;
    std::vector<unsigned char> answerIdData(data.begin(), data.end());
    bytesRead += answerId.deserialize(answerIdData);
    std::vector<unsigned char> questionIdData(data.begin() + bytesRead,
                                              data.end());
    bytesRead += questionId.deserialize(questionIdData);
    std::vector<unsigned char> userIdData(data.begin() + bytesRead, data.end());
    bytesRead += userId.deserialize(userIdData);
    std::vector<unsigned char> answerData(data.begin() + bytesRead, data.end());
    bytesRead += answer.deserialize(answerData);
    return bytesRead;
  }
  uint64_t getSize() {
    return answerId.getSize() + questionId.getSize() + userId.getSize() +
           answer.getSize();
  }
};
}; // namespace DTO