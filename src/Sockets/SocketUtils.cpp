#include "Sockets/SocketUtils.h"
#include "Sockets/SocketException.h"
#include <string>

void setTimeout(SOCKET socket, long sec, long uSec, int optionName) {
  int returnCode = 0;
  timeval timeout{sec, uSec};
  returnCode =
      setsockopt(socket, SOL_SOCKET, optionName, &timeout, sizeof(timeout));
  if (returnCode < 0) {
    throw SocketException(std::string("Failed to set") +
                              std::to_string(optionName) +
                              std::string("timeout on udp socket"),
                          socket, ErrorStatus::E_InternalServerError);
  }
}