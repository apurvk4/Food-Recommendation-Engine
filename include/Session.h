#pragma once

#include <cstdint>
#include <string>
struct Session {
  uint64_t userId;
  std::string sessionToken;
  std::string createdAt;
  std::string validTill;
  Session(uint64_t userId, std::string sessionToken, std::string createdAt,
          std::string validTill)
      : userId(userId), sessionToken(sessionToken), createdAt(createdAt),
        validTill(validTill) {}
};