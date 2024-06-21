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

namespace Server {

class RequestHandler {

  std::pair<TCPRequest, std::vector<unsigned char>>
  parseHeaders(std::vector<unsigned char> &data);
  void writeProtocolHeader(std::vector<unsigned char> &buffer,
                           ProtocolHeader &header);
  void handleLoginRequest(TcpSocket socket, TCPRequest request,
                          std::vector<unsigned char> payload);
  void handleGetFoodItemsRequest(TcpSocket socket, TCPRequest request,
                                 std::vector<unsigned char> payload);
  void handleRequest(TcpSocket socket, U32 requestId, TCPRequest request,
                     std::vector<unsigned char> payload);
  void handleAddFoodItemRequest(TcpSocket socket, TCPRequest request,
                                std::vector<unsigned char> payload);
  void handleUpdateFoodItemRequest(TcpSocket socket, TCPRequest request,
                                   std::vector<unsigned char> payload);
  void handleDeleteFoodItemRequest(TcpSocket socket, TCPRequest request,
                                   std::vector<unsigned char> payload);
  void handleFoodItemRecommendationRequest(TcpSocket socket, TCPRequest request,
                                           std::vector<unsigned char> payload);

public:
  RequestHandler(){};
  RequestHandler(const RequestHandler &) = delete;
  RequestHandler &operator=(const RequestHandler &) = delete;
  void handleRequest(TcpSocket socket);
  ~RequestHandler() = default;
};
}; // namespace Server