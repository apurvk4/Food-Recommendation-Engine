#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U32.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct Review : public Serializable {
  U64 ReviewId;
  U64 userId;
  U64 foodItemId;
  U32 rating;
  SString comment;
  SString date;

  Review()
      : ReviewId(0), userId(0), foodItemId(0), rating(0), comment(""),
        date("") {}
  Review(U64 ReviewId, U64 userId, U64 foodItemId, U32 rating,
         std::string comment, std::string date)
      : ReviewId(ReviewId), userId(userId), foodItemId(foodItemId),
        rating(rating), comment(comment), date(date) {}
  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> ReviewIdSerialized = ReviewId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> ratingSerialized = rating.serialize();
    std::vector<unsigned char> commentSerialized = comment.serialize();
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), ReviewIdSerialized.begin(),
                      ReviewIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
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
    uint64_t bytesRead = 0;
    std::vector<unsigned char> ReviewIdData(data.begin(), data.begin() + 8);
    bytesRead += ReviewId.deserialize(ReviewIdData);
    std::vector<unsigned char> userIdData(data.begin() + bytesRead,
                                          data.begin() + bytesRead + 8);
    bytesRead += userId.deserialize(userIdData);
    std::vector<unsigned char> foodItemIdData(data.begin() + bytesRead,
                                              data.begin() + bytesRead + 8);
    bytesRead += foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> ratingData(data.begin() + bytesRead,
                                          data.begin() + bytesRead + 4);
    bytesRead += rating.deserialize(ratingData);
    std::vector<unsigned char> commentData(data.begin() + bytesRead,
                                           data.end());
    bytesRead += comment.deserialize(commentData);
    return bytesRead;
  }
  std::size_t getSize() override {
    return ReviewId.getSize() + userId.getSize() + foodItemId.getSize() +
           rating.getSize() + comment.getSize() + date.getSize();
  }
};

}; // namespace DTO