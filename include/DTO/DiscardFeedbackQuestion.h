#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
namespace DTO {
struct DiscardFeedbackQuestion : public Serializable {
  U64 questionId;
  U64 foodItemId;
  SString question;
  DiscardFeedbackQuestion() : questionId(0), foodItemId(0), question("") {}
  DiscardFeedbackQuestion(U64 questionId, U64 foodItemId, SString question)
      : questionId(questionId), foodItemId(foodItemId), question(question) {}
  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> questionIdSerialized = questionId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> questionSerialized = question.serialize();
    serialized.insert(serialized.end(), questionIdSerialized.begin(),
                      questionIdSerialized.end());
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), questionSerialized.begin(),
                      questionSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) {
    uint64_t bytesRead = 0;
    std::vector<unsigned char> questionIdData(data.begin(), data.end());
    bytesRead += questionId.deserialize(questionIdData);
    std::vector<unsigned char> foodItemIdData(data.begin() + bytesRead,
                                              data.end());
    bytesRead += foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> questionData(data.begin() + bytesRead,
                                            data.end());
    bytesRead += question.deserialize(questionData);
    return bytesRead;
  }
  uint64_t getSize() {
    return questionId.getSize() + foodItemId.getSize() + question.getSize();
  }
};
}; // namespace DTO