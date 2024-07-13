#include "Sockets/Socket.h"
#include "Sockets/SocketException.h"
#include "Sockets/SocketUtils.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

Socket::Socket(SOCKET socket, in_addr remoteIp, uint16_t remotePort,
               uint16_t localPort, in_addr localIp)
    : socket{socket}, remoteIp{remoteIp}, remotePort{remotePort},
      localPort{localPort}, localIp{localIp} {}

in_addr Socket::getLocalIP() const { return localIp; }

uint16_t Socket::getLocalPort() const {
  // use getsockname to get the local port
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLen = sizeof(clientAddress);
  if (getsockname(socket, (sockaddr *)&clientAddress, &clientAddressLen) < 0) {
    throw SocketException("Failed to get local address",
                          ErrorStatus::E_InternalServerError);
  }
  return ntohs(clientAddress.sin_port);
}

uint16_t Socket::getRemotePort() const { return remotePort; }

in_addr Socket::getRemoteIP() const { return remoteIp; }

void Socket::close() {
  if (socket > 0) {
    ::close(socket);
  }
}

in_addr Socket::convertIpAddress(std::string &ipAddress) {
  sockaddr_in addr;
  if (inet_pton(AF_INET, ipAddress.c_str(), &(addr.sin_addr))) {
    return addr.sin_addr;
  } else if (inet_pton(AF_INET6, ipAddress.c_str(), &(addr.sin_addr))) {
    return addr.sin_addr;
  }
  throw std::invalid_argument("Invalid ip Address");
}

void Socket::bind(const std::string &localIpAddress, uint16_t localPort) {
  sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_port = localPort;
  std::string Ip{localIpAddress};
  address.sin_addr = Socket::convertIpAddress(Ip);
  if (address.sin_addr.s_addr == INADDR_NONE) {
    throw SocketException("Invalid IP Address",
                          ErrorStatus::E_InternalServerError);
  }
  if (::bind(socket, (sockaddr *)&address, sizeof(address)) < 0) {
    throw SocketException("Failed to bind socket",
                          ErrorStatus::E_InternalServerError);
  }
  localIp = address.sin_addr;
  sockaddr_in localAddress = {0};
  socklen_t socketAddrLen = sizeof(localAddress);
  if (getsockname(socket, (sockaddr *)&localAddress, &socketAddrLen) < 0) {
    throw SocketException("Failed to get local address",
                          ErrorStatus::E_InternalServerError);
  }
  this->localPort = localAddress.sin_port;
}

std::string Socket::convertIpAddress(in_addr &address) {
  char ip[INET_ADDRSTRLEN];
  char ip6[INET6_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &address, ip, INET_ADDRSTRLEN)) {
    return ip;
  } else if (inet_ntop(AF_INET6, &address, ip6, INET6_ADDRSTRLEN)) {
    return ip6;
  }
  throw std::invalid_argument("Invalid ip Address");
}

Socket::Socket(Socket &&other) noexcept
    : socket(other.socket), remoteIp(other.remoteIp),
      remotePort(other.remotePort), localPort(other.localPort),
      localIp(other.localIp) {
  other.socket = 0;
}

Socket &Socket::operator=(Socket &&other) noexcept {
  if (this != &other) {
    socket = other.socket;
    remoteIp = other.remoteIp;
    remotePort = other.remotePort;
    localPort = other.localPort;
    localIp = other.localIp;
    other.socket = 0;
  }
  return *this;
}

Socket::~Socket() { close(); }