#pragma once

#include "SerializableTypes/Array.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/SString.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

typedef std::pair<ErrorStatus, std::vector<unsigned char>> (*HandlerFn)(
    std::vector<unsigned char>, TCPRequest);

namespace Server {

class RequestHandler {
  std::unordered_map<std::string, HandlerFn> registeredEndpoints;
  std::pair<TCPRequest, std::vector<unsigned char>>
  parseHeaders(std::vector<unsigned char> &data);
  void writeResponseHeader(std::vector<unsigned char> &responseBuffer,
                           ResponseHeader &responseHeader);
  void writeProtocolHeader(std::vector<unsigned char> &buffer,
                           ProtocolHeader &header);

public:
  RequestHandler(){};
  RequestHandler(const RequestHandler &) = delete;
  RequestHandler &operator=(const RequestHandler &) = delete;
  void registerEndpoint(std::string endpoint, HandlerFn handler);
  void handleRequest(TcpSocket socket);
  ~RequestHandler() = default;
};
}; // namespace Server