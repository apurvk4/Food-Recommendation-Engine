#pragma once

#include "DbConnection.h"
#include "INotificationDAO.h"
#include "Notification.h"
#include <memory>

class NotificationDAO : public INotificationDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  NotificationDAO();
  bool sendNotification(Notification notification) override;
  Notification getNotification(uint64_t notificationId) override;
  bool deleteNotification(uint64_t notificationId) override;
  bool updateNotification(Notification notification) override;
  std::vector<Notification>
  getAllNotificationsAfter(uint64_t notificationId) override;
  ~NotificationDAO() = default;
};