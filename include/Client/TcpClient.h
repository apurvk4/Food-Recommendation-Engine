#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

class TcpClient {
public:
  TcpClient(const std::string &serverIp, int serverPort);
  ~TcpClient();
  bool connectToServer();
  bool sendData(const std::vector<unsigned char> &data);
  std::vector<unsigned char> receiveData();
  int getLocalPort();

private:
  std::string serverIp_;
  int serverPort_;
  int sock_;
  struct sockaddr_in serv_addr_;
};
