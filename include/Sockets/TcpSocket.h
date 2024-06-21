#pragma once

#include "Socket.h"
#include "SocketException.h"
#include "SocketUtils.h"
#include <cstring>
#include <iostream>
#include <vector>

class TcpSocket : public Socket {
  void initializeSocket();

public:
  TcpSocket(SOCKET socket, std::string remoteIp, uint16_t remotePort,
            uint16_t localPort, std::string localIp);
  TcpSocket(std::string remoteIp, uint16_t remotePort);
  TcpSocket(TcpSocket &&other) = default;
  TcpSocket &operator=(TcpSocket &&other) = default;
  TcpSocket(const TcpSocket &) = delete;
  TcpSocket &operator=(const TcpSocket &) = delete;
  void connect();

  bool isConnected();

  template <typename T> T receive();

  template <typename T> void send(T &dataToSend);
  void shutdown(int shutdownFlag) { ::shutdown(socket, shutdownFlag); }

  std::vector<unsigned char> receiveData();
  void sendData(std::vector<unsigned char> &dataToSend);

  void close();
  ~TcpSocket();
};

template <typename T> T TcpSocket::receive() {
  T dataToReceive;
  memset((void *)&dataToReceive, 0, sizeof(dataToReceive));

  int totalBytesReceived = 0;
  int bytesReceived = 0;
  do {
    bytesReceived = ::recv(socket, (char *)&dataToReceive + totalBytesReceived,
                           sizeof(dataToReceive) - totalBytesReceived, 0);
    totalBytesReceived += bytesReceived;
  } while (bytesReceived != -1);

  if (bytesReceived != sizeof(dataToReceive)) {
    throw SocketException(
        std::string("Failed to read the required data from socket, Read : ") +
            std::to_string(bytesReceived) + " bytes Needed :" +
            std::to_string(sizeof(dataToReceive)) + " bytes.",
        socket, ErrorStatus::E_TcpReceiveError);
  }
  return dataToReceive;
}

template <typename T> void TcpSocket::send(T &dataToSend) {
  int totalBytesSend = 0;
  int bytesSend = 0;
  do {
    bytesSend = ::send(socket, (char *)&dataToSend + totalBytesSend,
                       sizeof(dataToSend) - totalBytesSend, 0);
    totalBytesSend += bytesSend;
  } while (bytesSend != -1);

  if (totalBytesSend == SOCKET_ERROR) {
    throw SocketException("Failed to send Data to remote ", socket,
                          ErrorStatus::E_TcpSendError);
  }
}