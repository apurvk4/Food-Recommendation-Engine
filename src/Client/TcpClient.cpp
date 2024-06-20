#include "Client/TcpClient.h"

TcpClient::TcpClient(const std::string &serverIp, int serverPort)
    : serverIp_(serverIp), serverPort_(serverPort), sock_(0) {
  memset(&serv_addr_, 0, sizeof(serv_addr_));
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(serverPort_);
}

TcpClient::~TcpClient() {
  if (sock_ > 0) {
    close(sock_);
  }
}

bool TcpClient::connectToServer() {
  if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return false;
  }

  if (inet_pton(AF_INET, serverIp_.c_str(), &serv_addr_.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return false;
  }

  if (connect(sock_, (struct sockaddr *)&serv_addr_, sizeof(serv_addr_)) < 0) {
    std::cerr << "Connection Failed" << std::endl;
    return false;
  }

  return true;
}

int TcpClient::getLocalPort() {
  struct sockaddr_in localAddr;
  socklen_t addrLen = sizeof(localAddr);

  if (getsockname(sock_, (struct sockaddr *)&localAddr, &addrLen) == -1) {
    std::cerr << "getsockname() failed" << std::endl;
    return -1;
  }

  return ntohs(localAddr.sin_port);
}

bool TcpClient::sendData(const std::vector<unsigned char> &data) {
  if (send(sock_, data.data(), data.size(), 0) < 0) {
    std::cerr << "Send failed" << std::endl;
    return false;
  }
  ::shutdown(sock_, SHUT_WR);
  return true;
}

std::vector<unsigned char> TcpClient::receiveData() {
  std::vector<unsigned char> receivedData;
  char buffer[1024];
  int valread;

  while ((valread = read(sock_, buffer, sizeof(buffer))) > 0) {
    receivedData.insert(receivedData.end(), buffer, buffer + valread);
  }

  if (valread < 0) {
    std::cerr << "Read error" << std::endl;
  }

  return receivedData;
}