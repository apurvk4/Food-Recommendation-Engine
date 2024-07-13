#pragma once

#include "SerializableTypes/Array.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/SString.h"
#include "Server/RouteHandler.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace Server {

class RequestHandler {
  std::unique_ptr<RouteHandler> routeHandler;
  std::pair<TCPRequest, std::vector<unsigned char>>
  parseHeaders(std::vector<unsigned char> &data);

public:
  RequestHandler(std::unique_ptr<RouteHandler> routeHandler)
      : routeHandler(std::move(routeHandler)){};
  RequestHandler(const RequestHandler &) = delete;
  RequestHandler &operator=(const RequestHandler &) = delete;
  void handleRequest(TcpSocket socket);
  ~RequestHandler() = default;
};
}; // namespace Server