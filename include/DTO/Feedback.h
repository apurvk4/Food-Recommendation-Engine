#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>
namespace DTO {

struct Feedback {
  U64 feedbackId;
  U64 userId;
  U64 foodItemId;
  bool preference;
  SString date;
  Feedback()
      : preference{false}, date{""}, userId{0}, feedbackId{0}, foodItemId{0} {}
  Feedback(U64 feedbackId, U64 userId, U64 foodItemId, bool preference,
           std::string date)
      : feedbackId(feedbackId), userId(userId), foodItemId(foodItemId),
        preference(preference), date(date) {}

  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> feedbackIdSerialized = feedbackId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> preferenceSerialized;
    preferenceSerialized.push_back(preference);
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), feedbackIdSerialized.begin(),
                      feedbackIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), preferenceSerialized.begin(),
                      preferenceSerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) {
    uint64_t bytesRead = 0;
    bytesRead += feedbackId.deserialize(data);
    bytesRead += userId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += foodItemId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    preference = data[bytesRead];
    bytesRead++;
    bytesRead += date.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() {
    return feedbackId.getSize() + userId.getSize() + foodItemId.getSize() + 1 +
           date.getSize();
  }
};

}; // namespace DTO