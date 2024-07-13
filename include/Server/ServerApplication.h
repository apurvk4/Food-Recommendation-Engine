#pragma once

#include "IApplication.h"
#include "RequestHandler.h"
#include "Server/LoadBalancer.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

namespace Server {

class ServerApplication : public IApplication {
  LoadBalancer loadBalancer;
  const int connectionBacklog;
  sockaddr_in serverAddress;
  SOCKET serverSocket;
  std::unique_ptr<RequestHandler> requestHandler;
  in_addr serverIp;
  uint16_t portNumber;
  std::atomic<bool> isRunning;
  void cleanUp(SOCKET socket);
  void initializeServer(in_addr serverIp, uint16_t portNumber);
  bool didSocketTimeout(SOCKET socket);
  void handleAcceptedSocket(SOCKET acceptedSocket, sockaddr_in remoteAddress);
  bool isSocketValid(SOCKET socket);

public:
  ServerApplication(std::unique_ptr<RequestHandler> requestHandler,
                    in_addr serverIp, uint16_t portNumber,
                    int connectionBacklog = 5);
  void start() override;
  void stop() override;
  ~ServerApplication();
};
}; // namespace Server