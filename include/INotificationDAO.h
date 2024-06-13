#pragma once

#include "Notification.h"
#include <cstdint>
#include <vector>

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