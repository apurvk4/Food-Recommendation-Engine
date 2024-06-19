#pragma once

#include <cstdint>
#include <string>
namespace DTO {
struct ScheduledMenu {
  uint64_t scheduledMenuId;
  uint64_t menuId;
  std::string date;
  bool isSurveyMenu;
  ScheduledMenu(uint64_t scheduledMenuId, uint64_t menuId, std::string date,
                bool isSurveyMenu)
      : scheduledMenuId(scheduledMenuId), menuId(menuId), date(date),
        isSurveyMenu(isSurveyMenu) {}
};
}; // namespace DTO