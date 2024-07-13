#include "Server/FoodRecommendationServer.h"

#include "Server/LoadBalancer.h"
#include "Sockets/Socket.h"
#include "Sockets/SocketUtils.h"
#include <cstring>

using Server::FoodRecommendationServer;

void FoodRecommendationServer::cleanUp(SOCKET socket) {
  if (socket > 0) {
    closeSocket(socket);
  }
}

FoodRecommendationServer::~FoodRecommendationServer() { cleanUp(serverSocket); }

void FoodRecommendationServer::initializeServer(in_addr serverIp,
                                                uint16_t portNumber) {
  memset(&(serverAddress), 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portNumber);
  serverAddress.sin_addr = serverIp;
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    throw SocketException(std::string("Failed to create server Socket : ") +
                              std::to_string(serverSocket),
                          ErrorStatus::E_InternalServerError);
  }
  int returnValue =
      bind(serverSocket, (sockaddr *)&serverAddress, sizeof(sockaddr));
  if (returnValue < 0) {
    throw SocketException(std::string("Failed to bind socket : ") +
                              std::string("on port : ") +
                              std::to_string(portNumber),
                          serverSocket, ErrorStatus::E_InternalServerError);
  }
  returnValue = listen(serverSocket, connectionBacklog);
  if (returnValue < 0) {
    throw SocketException(std::string("Failed to listen on socket : ") +
                              std::to_string(returnValue),
                          ErrorStatus::E_InternalServerError);
  }
}

bool FoodRecommendationServer::didSocketTimeout(SOCKET socket) {
  if (errno == EAGAIN || errno == EWOULDBLOCK) {
    return true;
  }
  return false;
}

bool FoodRecommendationServer::isSocketValid(SOCKET socket) {
  if (socket <= 0) {
    if (didSocketTimeout(socket)) {
      return false;
    }
    throw SocketException("Failed to accept socket ", socket,
                          ErrorStatus::E_InternalServerError);
  }
  return true;
}

void FoodRecommendationServer::handleAcceptedSocket(SOCKET acceptedSocket,
                                                    sockaddr_in remoteAddress) {
  if (!isSocketValid(acceptedSocket)) {
    return;
  }
  try {
    std::cout << "\nConnected to client\n";
    TcpSocket socket{acceptedSocket,
                     Socket::convertIpAddress(remoteAddress.sin_addr),
                     remoteAddress.sin_port, serverAddress.sin_port,
                     Socket::convertIpAddress(serverAddress.sin_addr)};
    requestHandler.handleRequest(std::move(socket));
  } catch (std::exception &exception) {
    std::cerr << "Error in handling request on client socketId "
              << acceptedSocket << " : " << exception.what();
  }
}

void FoodRecommendationServer::start(in_addr serverIp, uint16_t portNumber) {
  if (isRunning) {
    return;
  }
  initializeServer(serverIp, portNumber);
  std::cout << "Listening on Port Number : " << portNumber;
  socklen_t size = sizeof(sockaddr_in);
  isRunning = true;
  std::cout << "Server Started. Waiting for clients to connect";
  SOCKET acceptedSocket = 0;
  while (isRunning) {
    sockaddr_in remoteAddress;
    memset(&remoteAddress, 0, sizeof(remoteAddress));
    acceptedSocket = accept(serverSocket, (sockaddr *)&remoteAddress, &size);
    loadBalancer.distributeTask(
        [this, &acceptedSocket, &remoteAddress]() mutable {
          handleAcceptedSocket(acceptedSocket, remoteAddress);
        });
  }
}

void FoodRecommendationServer::stop() { isRunning = false; }

FoodRecommendationServer::FoodRecommendationServer(
    RequestHandler &connectionHandler, int connectionBacklog = 5)
    : connectionBacklog{connectionBacklog}, requestHandler{connectionHandler},
      isRunning{false}, loadBalancer(std::thread::hardware_concurrency() - 1) {}