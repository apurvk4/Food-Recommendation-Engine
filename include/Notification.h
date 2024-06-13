#pragma once

#include <cstdint>
#include <string>
struct Notification {
  uint64_t notificationId;
  std::string message;
  std::string date;
  Notification(uint64_t notificationId, std::string message, std::string date)
      : notificationId(notificationId), message(message), date(date) {}
};