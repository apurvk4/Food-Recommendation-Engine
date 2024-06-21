#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>
namespace DTO {

struct Preference {
  U64 preferenceId;
  U64 userId;
  U64 foodItemId;
  U64 menuId;
  bool preference;
  SString date;
  Preference(U64 preferenceId, U64 userId, U64 foodItemId, U64 menuId,
             bool preference, std::string date)
      : preferenceId(preferenceId), userId(userId), foodItemId(foodItemId),
        menuId(menuId), preference(preference), date(date) {}

  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> preferenceIdSerialized =
        preferenceId.serialize();
    std::vector<unsigned char> userIdSerialized = userId.serialize();
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> preferenceSerialized;
    preferenceSerialized.push_back(preference);
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), preferenceIdSerialized.begin(),
                      preferenceIdSerialized.end());
    serialized.insert(serialized.end(), userIdSerialized.begin(),
                      userIdSerialized.end());
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), preferenceSerialized.begin(),
                      preferenceSerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) {
    std::vector<unsigned char> preferenceIdData(data.begin(), data.begin() + 8);
    preferenceId.deserialize(preferenceIdData);
    std::vector<unsigned char> userIdData(data.begin() + 8, data.begin() + 16);
    userId.deserialize(userIdData);
    std::vector<unsigned char> foodItemIdData(data.begin() + 16,
                                              data.begin() + 24);
    foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> menuIdData(data.begin() + 24, data.begin() + 32);
    menuId.deserialize(menuIdData);
    preference = data[32];
    std::vector<unsigned char> dateData(data.begin() + 33, data.begin() + 65);
    date.deserialize(dateData);
    return 65;
  }
  size_t getSize() { return 65; }
};

}; // namespace DTO