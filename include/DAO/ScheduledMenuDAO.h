#pragma once

#include "DAO/IScheduledMenuDAO.h"
#include "DbConnection.h"
#include <cstdint>
#include <string>

namespace DAO {

class ScheduledMenuDAO : public IScheduledMenuDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  ScheduledMenuDAO();
  std::vector<DTO::ScheduledMenu> getAllScheduledMenus() override;
  DTO::ScheduledMenu getScheduledMenu(uint64_t scheduledMenuId) override;
  bool addScheduledMenu(DTO::ScheduledMenu scheduledMenu) override;
  bool updateScheduledMenu(DTO::ScheduledMenu scheduledMenu) override;
  bool deleteScheduledMenu(uint64_t scheduledMenuId) override;
  std::vector<DTO::ScheduledMenu>
  getScheduledMenuByDate(std::string date) override;
};

}; // namespace DAO