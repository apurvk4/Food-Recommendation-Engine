#pragma once

#include <cstdint>
#include <string>

class ISessionDAO {
public:
  virtual std::pair<bool, std::string> createSession(uint64_t userId) = 0;
  virtual bool deleteSession(std::string sessionToken) = 0;
  virtual uint64_t getUserIdBySessionToken(std::string sessionToken) = 0;
  virtual bool isSessionValid(std::string sessionToken) = 0;
  virtual bool updateSession(std::string sessionToken) = 0;
  virtual ~ISessionDAO() = default;
};