#pragma once

#include "RequestHandler.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

namespace Server {

class FoodRecommendationServer {
  const int connectionBacklog;
  sockaddr_in serverAddress;
  SOCKET serverSocket;
  RequestHandler &requestHandler;
  bool isRunning;
  void cleanUp(SOCKET socket);
  void initializeServer(in_addr serverIp, uint16_t portNumber);
  bool didSocketTimeout(SOCKET socket);
  void handleAcceptedSocket(SOCKET acceptedSocket, sockaddr_in remoteAddress);
  bool isSocketValid(SOCKET socket);

public:
  FoodRecommendationServer(RequestHandler &connectionHandler,
                           int connectionBacklog);
  void start(in_addr serverIp, uint16_t portNumber);
  void stop();
  ~FoodRecommendationServer();
};
}; // namespace Server