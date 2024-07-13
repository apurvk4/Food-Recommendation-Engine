#pragma once

#include "SocketUtils.h"
#include <exception>
#include <string>

class SocketException : public std::exception {
  ErrorStatus errorStatus;
  std::string message;
  SOCKET socket;

public:
  SocketException(const std::string &message, SOCKET socket,
                  ErrorStatus errorStatus);
  SocketException(const std::string &message, ErrorStatus errorStatus);
  std::string getErrorDescription();
  ErrorStatus getErrorStatus();
  const char *what() const throw();
};