#pragma once

#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"

namespace DTO {

struct UserActivity : public Serializable {
  U64 activityId;
  U64 userId;
  U64 activityTypeId;
  U64 activityTime;

  UserActivity() = default;
  UserActivity(uint64_t activityId, uint64_t userId, uint64_t activityTypeId,
               uint64_t activityTime)
      : activityId(activityId), userId(userId), activityTypeId(activityTypeId),
        activityTime(activityTime) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> activityIdSerialized = activityId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> activityTypeSerialized =
        activityTypeId.serialize();
    std::vector<unsigned char> timestampSerialized = activityTime.serialize();
    serialized.insert(serialized.end(), activityIdSerialized.begin(),
                      activityIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), activityTypeSerialized.begin(),
                      activityTypeSerialized.end());
    serialized.insert(serialized.end(), timestampSerialized.begin(),
                      timestampSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> activityIdData(data.begin(), data.begin() + 8);
    uint64_t bytesRead = activityId.deserialize(activityIdData);
    std::vector<unsigned char> userIdData(data.begin() + 8, data.begin() + 16);
    bytesRead += userId.deserialize(userIdData);
    std::vector<unsigned char> activityTypeData(data.begin() + 16,
                                                data.begin() + 24);
    bytesRead += activityTypeId.deserialize(activityTypeData);
    std::vector<unsigned char> timestampData(data.begin() + 24,
                                             data.begin() + 32);
    bytesRead += activityTime.deserialize(timestampData);
    return bytesRead;
  }
  size_t getSize() override {
    return activityId.getSize() + userId.getSize() + activityTypeId.getSize() +
           activityTime.getSize();
  }
};
}; // namespace DTO