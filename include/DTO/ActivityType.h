#pragma once

#include "SerializableTypes/SString.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"

namespace DTO {

enum class ActivityType : uint64_t {
  Login = 1,
  Logout = 2,
};

inline std::string ActivityTypeToString(ActivityType activityType) {
  switch (activityType) {
  case ActivityType::Login:
    return "Login";
  case ActivityType::Logout:
    return "Logout";
  default:
    throw std::invalid_argument("Invalid activity type");
  }
}

inline ActivityType StringToActivityType(const std::string &activityType) {
  if (activityType == "Login") {
    return ActivityType::Login;
  } else if (activityType == "Logout") {
    return ActivityType::Logout;
  } else {
    throw std::invalid_argument("Invalid activity type");
  }
}

}; // namespace DTO