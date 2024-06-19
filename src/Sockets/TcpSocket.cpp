#include "Sockets/TcpSocket.h"
#include "Sockets/SocketException.h"
#include "Sockets/SocketUtils.h"
#include <cstring>

TcpSocket::TcpSocket(SOCKET socket, std::string remoteIp, uint16_t remotePort,
                     uint16_t localPort, std::string localIp)
    : Socket{socket, convertIpAddress(remoteIp), remotePort, localPort,
             convertIpAddress(localIp)} {}

TcpSocket::TcpSocket(std::string remoteIp, uint16_t remotePort) {
  this->socket = 0;
  this->remoteIp = convertIpAddress(remoteIp);
  this->remotePort = remotePort;
  this->localPort = 0;
  this->localIp = {0};
  initializeSocket();
}

void TcpSocket::initializeSocket() {
  socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket <= 0) {
    throw SocketException("Failed to create socket",
                          ErrorStatus::E_InternalServerError);
  }
}

void TcpSocket::connect() {
  if (socket <= 0) {
    throw SocketException("Socket is not initialized",
                          ErrorStatus::E_InternalServerError);
  }
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = remotePort;
  addr.sin_addr = remoteIp;
  if (::connect(socket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    throw SocketException("Failed to connect", socket,
                          ErrorStatus::E_InternalServerError);
  }
}

bool TcpSocket::isConnected() {
  int error = 0;
  socklen_t length = sizeof(error);
  getsockopt(socket, SOL_SOCKET, SO_ERROR, (char *)&error, &length);
  return error == 0;
}

void TcpSocket::close() {
  if (socket > 0) {
    ::shutdown(socket, SHUTDOWN_BOTH);
  }
}

TcpSocket::~TcpSocket() { close(); }

std::vector<unsigned char> TcpSocket::receiveData() {
  // read all the data from socket until the receiving data fails with 0 or -1

  std::vector<unsigned char> data;
  unsigned char buffer[1024];
  int bytesReceived = 0;
  do {
    bytesReceived = ::recv(socket, (char *)buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
      data.insert(data.end(), buffer, buffer + bytesReceived);
    }
  } while (bytesReceived > 0);
  return data;
}

void TcpSocket::sendData(std::vector<unsigned char> &dataToSend) {
  int totalBytesSend = 0;
  int bytesSend = 0;
  do {
    bytesSend = ::send(socket, (char *)dataToSend.data() + totalBytesSend,
                       dataToSend.size() - totalBytesSend, 0);
    if (bytesSend > 0) {
      totalBytesSend += bytesSend;
    }
  } while (bytesSend != -1);
  if (totalBytesSend == SOCKET_ERROR) {
    throw SocketException("Failed to send data", socket,
                          ErrorStatus::E_TcpSendError);
  }
}