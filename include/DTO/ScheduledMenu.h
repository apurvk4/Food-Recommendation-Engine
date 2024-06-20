#pragma once

#include "SerializableTypes/SString.h"
#include <cstdint>
#include <string>
namespace DTO {
struct ScheduledMenu {
  U64 scheduledMenuId;
  U64 menuId;
  SString date;
  bool isSurveyMenu;
  ScheduledMenu(U64 scheduledMenuId, U64 menuId, SString date,
                bool isSurveyMenu)
      : scheduledMenuId(scheduledMenuId), menuId(menuId), date(date),
        isSurveyMenu(isSurveyMenu) {}

  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> scheduledMenuIdSerialized =
        scheduledMenuId.serialize();
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> dateSerialized = date.serialize();
    std::vector<unsigned char> isSurveyMenuSerialized;
    isSurveyMenuSerialized.push_back(isSurveyMenu);
    serialized.insert(serialized.end(), scheduledMenuIdSerialized.begin(),
                      scheduledMenuIdSerialized.end());
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    serialized.insert(serialized.end(), isSurveyMenuSerialized.begin(),
                      isSurveyMenuSerialized.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) {
    std::vector<unsigned char> scheduledMenuIdData(data.begin(),
                                                   data.begin() + 8);
    scheduledMenuId.deserialize(scheduledMenuIdData);
    std::vector<unsigned char> menuIdData(data.begin() + 8, data.begin() + 16);
    menuId.deserialize(menuIdData);
    std::vector<unsigned char> dateData(data.begin() + 16, data.begin() + 48);
    date.deserialize(dateData);
    isSurveyMenu = data[48];
    return 49;
  }

  size_t getSize() { return 49; }
};
}; // namespace DTO