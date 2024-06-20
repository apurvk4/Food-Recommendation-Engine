#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U32.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct Feedback : public Serializable {
  U64 feedbackId;
  U64 userId;
  U64 menuId;
  U64 foodItemId;
  U32 rating;
  SString comment;
  SString date;
  Feedback(U64 feedbackId, U64 userId, U64 menuId, U64 foodItemId, U32 rating,
           SString comment, SString date)
      : feedbackId(feedbackId), userId(userId), menuId(menuId),
        foodItemId(foodItemId), rating(rating), comment(comment), date(date) {}
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> feedbackIdSerialized = feedbackId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> ratingSerialized = rating.serialize();
    std::vector<unsigned char> commentSerialized = comment.serialize();
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), feedbackIdSerialized.begin(),
                      feedbackIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), ratingSerialized.begin(),
                      ratingSerialized.end());
    serialized.insert(serialized.end(), commentSerialized.begin(),
                      commentSerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> feedbackIdData(data.begin(), data.begin() + 8);
    feedbackId.deserialize(feedbackIdData);
    std::vector<unsigned char> userIdData(data.begin() + 8, data.begin() + 16);
    userId.deserialize(userIdData);
    std::vector<unsigned char> menuIdData(data.begin() + 16, data.begin() + 24);
    menuId.deserialize(menuIdData);
    std::vector<unsigned char> foodItemIdData(data.begin() + 24,
                                              data.begin() + 32);
    foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> ratingData(data.begin() + 32, data.begin() + 36);
    rating.deserialize(ratingData);
    std::vector<unsigned char> commentData(data.begin() + 36,
                                           data.begin() + 68);
    comment.deserialize(commentData);
    std::vector<unsigned char> dateData(data.begin() + 68, data.begin() + 100);
    date.deserialize(dateData);
    return 100;
  }
  std::size_t getSize() override {
    return feedbackId.getSize() + userId.getSize() + menuId.getSize() +
           foodItemId.getSize() + rating.getSize() + comment.getSize() +
           date.getSize();
  }
};

}; // namespace DTO