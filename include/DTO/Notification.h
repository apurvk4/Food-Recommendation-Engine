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
  Notification() : notificationId(0), message(""), date("") {}
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
    uint64_t bytesRead = 0;
    notificationId.deserialize(
        std::vector<unsigned char>(data.begin(), data.begin() + 8));
    bytesRead += 8;
    bytesRead += message.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    bytesRead += date.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() override {
    return notificationId.getSize() + message.getSize() + date.getSize();
  }
};

}; // namespace DTO