#pragma once

#include "DTO/Notification.h"
#include <cstdint>
#include <vector>

namespace DAO {

using DTO::Notification;

class INotificationDAO {

public:
  virtual bool sendNotification(Notification) = 0;
  virtual Notification getNotification(uint64_t notificationId) = 0;
  virtual bool deleteNotification(uint64_t notificationId) = 0;
  virtual bool updateNotification(Notification) = 0;
  virtual std::vector<Notification>
  getAllNotificationsAfter(uint64_t notificationId) = 0;
  virtual ~INotificationDAO() = default;
};

}; // namespace DAO