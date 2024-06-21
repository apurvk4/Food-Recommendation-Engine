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
                  ErrorStatus errorStatus) {
    this->message = message;
    this->socket = socket;
    this->errorStatus = errorStatus;
  }
  SocketException(const std::string &message, ErrorStatus errorStatus) {
    this->message = message;
    this->errorStatus = errorStatus;
  }
  std::string getErrorDescription() {
    return message + " : " + std::to_string(socket);
  }
  ErrorStatus getErrorStatus() { return errorStatus; }
  const char *what() const throw() { return message.c_str(); }
};