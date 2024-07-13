#include "Client/TcpClient.h"

TcpClient::TcpClient(const std::string &serverIp, int serverPort)
    : serverIp(serverIp), serverPort(serverPort), sock(0) {
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(serverPort);
}

TcpClient::~TcpClient() {
  if (sock > 0) {
    close(sock);
  }
}

bool TcpClient::connectToServer() {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return false;
  }
  if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return false;
  }

  if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cerr << "Connection Failed" << std::endl;
    return false;
  }

  return true;
}

int TcpClient::getLocalPort() {
  struct sockaddr_in localAddr;
  socklen_t addrLen = sizeof(localAddr);

  if (getsockname(sock, (struct sockaddr *)&localAddr, &addrLen) == -1) {
    std::cerr << "getsockname() failed" << std::endl;
    return -1;
  }

  return ntohs(localAddr.sin_port);
}

bool TcpClient::sendData(const std::vector<unsigned char> &data) {
  if (send(sock, data.data(), data.size(), 0) < 0) {
    std::cerr << "Send failed" << std::endl;
    return false;
  }
  ::shutdown(sock, SHUT_WR);
  return true;
}

std::vector<unsigned char> TcpClient::receiveData() {
  std::vector<unsigned char> receivedData;
  char buffer[1024];
  int valread;

  while ((valread = read(sock, buffer, sizeof(buffer))) > 0) {
    receivedData.insert(receivedData.end(), buffer, buffer + valread);
  }

  if (valread < 0) {
    std::cerr << "Read error" << std::endl;
  }

  return receivedData;
}