#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include <cstdint>
#include <string>

namespace DTO {

struct Notification : public Serializable {
  U64 notificationId;
  SString message;
  SString date;
  Notification(U64 notificationId, SString message, SString date)
      : notificationId(notificationId), message(message), date(date) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> notificationIdSerialized =
        notificationId.serialize();
    std::vector<unsigned char> messageSerialized = message.serialize();
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), notificationIdSerialized.begin(),
                      notificationIdSerialized.end());
    serialized.insert(serialized.end(), messageSerialized.begin(),
                      messageSerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    std::vector<unsigned char> notificationIdData(data.begin(),
                                                  data.begin() + 8);
    notificationId.deserialize(notificationIdData);
    std::vector<unsigned char> messageData(data.begin() + 8, data.begin() + 40);
    message.deserialize(messageData);
    std::vector<unsigned char> dateData(data.begin() + 40, data.begin() + 72);
    date.deserialize(dateData);
    return 72;
  }
  size_t getSize() override { return 72; }
};

}; // namespace DTO