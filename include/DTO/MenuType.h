#pragma once

#include <cstdint>
#include <string>

namespace DTO {

class MenuType {
public:
  uint64_t menuTypeId;
  std::string menuType;
  MenuType(uint64_t menuTypeId, std::string menuType)
      : menuTypeId(menuTypeId), menuType(menuType) {}
};

}; // namespace DTO