#pragma once
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdint>
#include <errno.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef int SOCKET;

class Socket {
protected:
  SOCKET socket;
  in_addr remoteIp;
  uint16_t remotePort;
  uint16_t localPort;
  in_addr localIp;
  Socket(SOCKET socket, in_addr remoteIp, uint16_t remotePort,
         uint16_t localPort, in_addr localIp);
  Socket() = default;

public:
  static in_addr convertIpAddress(std::string &ipAddress);
  static std::string convertIpAddress(in_addr &);
  Socket(const Socket &) = delete;
  Socket operator=(const Socket &) = delete;
  uint16_t getRemotePort() const;
  uint16_t getLocalPort() const;
  in_addr getLocalIP() const;
  in_addr getRemoteIP() const;
  void close();
  Socket(Socket &&other) noexcept;
  void setTimeout(long sec, long uSec, int optionName);
  void bind(const std::string &localIpAddress, uint16_t port);
  Socket &operator=(Socket &&other) noexcept;
  ~Socket();
};