#pragma once

#include "SerializableTypes/ProtocolDefinitions.h"
#include "Sockets/TcpSocket.h"
#include <memory>

namespace Middleware {

class IMiddleware {
public:
  virtual bool handleRequest(std::shared_ptr<TcpSocket> socket,
                             TCPRequest request,
                             std::vector<unsigned char> &) = 0;
  virtual ~IMiddleware() = default;
};

}; // namespace Middleware