#include "Server/ServerApplication.h"

#include "Server/LoadBalancer.h"
#include "Sockets/Socket.h"
#include "Sockets/SocketUtils.h"
#include <cstring>

using Server::ServerApplication;

void ServerApplication::cleanUp(SOCKET socket) {
  if (socket > 0) {
    closeSocket(socket);
  }
}

ServerApplication::~ServerApplication() { cleanUp(serverSocket); }

void ServerApplication::initializeServer(in_addr serverIp,
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

bool ServerApplication::didSocketTimeout(SOCKET socket) {
  if (errno == EAGAIN || errno == EWOULDBLOCK) {
    return true;
  }
  return false;
}

bool ServerApplication::isSocketValid(SOCKET socket) {
  if (socket <= 0) {
    if (didSocketTimeout(socket)) {
      return false;
    }
    throw SocketException("Failed to accept socket ", socket,
                          ErrorStatus::E_InternalServerError);
  }
  return true;
}

void ServerApplication::handleAcceptedSocket(SOCKET acceptedSocket,
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
    requestHandler->handleRequest(std::move(socket));
  } catch (std::exception &exception) {
    std::cerr << "Error in handling request on client socketId "
              << acceptedSocket << " : " << exception.what();
  }
}

void ServerApplication::start() {
  if (isRunning) {
    return;
  }
  initializeServer(serverIp, portNumber);
  std::cout << "Listening on Port Number : " << portNumber << std::endl;
  socklen_t size = sizeof(sockaddr_in);
  isRunning = true;
  std::cout << "Server Started. Waiting for clients to connect" << std::endl;
  SOCKET acceptedSocket = 0;

  while (isRunning.load()) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(serverSocket, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int ready = select(serverSocket + 1, &readfds, NULL, NULL, &timeout);
    if (ready == -1) {
      perror("select");
      break;
    } else if (ready == 0) {
      continue;
    }

    if (FD_ISSET(serverSocket, &readfds)) {
      sockaddr_in remoteAddress;
      memset(&remoteAddress, 0, sizeof(remoteAddress));
      acceptedSocket = accept(serverSocket, (sockaddr *)&remoteAddress, &size);
      if (acceptedSocket == -1) {
        perror("accept");
        continue;
      }
      loadBalancer.distributeTask(
          [this, acceptedSocket, remoteAddress]() mutable {
            handleAcceptedSocket(acceptedSocket, remoteAddress);
          });
    }
  }
  std::cout << "Server shutting down..." << std::endl;
  close(serverSocket);
  isRunning.store(false);
}

void ServerApplication::stop() {
  isRunning.store(false);
  std::cout << "stopping server\n";
}

ServerApplication::ServerApplication(
    std::unique_ptr<RequestHandler> requestHandler, in_addr serverIp,
    uint16_t portNumber, int connectionBacklog)
    : connectionBacklog{connectionBacklog},
      requestHandler{std::move(requestHandler)}, isRunning{false},
      loadBalancer(std::thread::hardware_concurrency() - 1), serverIp{serverIp},
      portNumber{portNumber} {}