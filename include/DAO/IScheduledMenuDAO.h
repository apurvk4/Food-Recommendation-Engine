#pragma once

#include "DTO/ScheduledMenu.h"
#include <vector>

namespace DAO {

class IScheduledMenuDAO {
public:
  virtual ~IScheduledMenuDAO() = default;
  virtual std::vector<DTO::ScheduledMenu> getAllScheduledMenus() = 0;
  virtual DTO::ScheduledMenu getScheduledMenu(uint64_t scheduledMenuId) = 0;
  virtual bool addScheduledMenu(DTO::ScheduledMenu scheduledMenu) = 0;
  virtual bool updateScheduledMenu(DTO::ScheduledMenu scheduledMenu) = 0;
  virtual bool deleteScheduledMenu(uint64_t scheduledMenuId) = 0;
  virtual std::vector<DTO::ScheduledMenu>
  getScheduledMenuByDate(std::string date) = 0;
};

}; // namespace DAO