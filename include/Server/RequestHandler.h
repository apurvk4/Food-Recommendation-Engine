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
#include <string>
#include <unordered_map>

namespace Server {

class RequestHandler {
  RouteHandler &routeHandler;
  std::pair<TCPRequest, std::vector<unsigned char>>
  parseHeaders(std::vector<unsigned char> &data);

public:
  RequestHandler(RouteHandler &routeHandler) : routeHandler(routeHandler){};
  RequestHandler(const RequestHandler &) = delete;
  RequestHandler &operator=(const RequestHandler &) = delete;
  void handleRequest(TcpSocket socket);
  ~RequestHandler() = default;
};
}; // namespace Server