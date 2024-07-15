#pragma once

#include "DTO/Notification.h"
#include "INotificationDAO.h"
#include "Server/DbConnection.h"
#include <memory>


namespace DAO {

class NotificationDAO : public INotificationDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  NotificationDAO();
  bool addNotification(Notification notification) override;
  Notification getNotification(uint64_t notificationId) override;
  bool deleteNotification(uint64_t notificationId) override;
  bool updateNotification(Notification notification) override;
  std::vector<Notification>
  getAllNotificationsAfter(uint64_t notificationId) override;
  ~NotificationDAO() = default;
};

}; // namespace DAO