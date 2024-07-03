#pragma once

#include "SerializableTypes/ProtocolDefinitions.h"
#include "TcpClient.h"
#include <sys/types.h>

class ClientCommunicator {
  TcpClient client;
  std::string serverAddress;
  uint16_t serverPort;
  void writeHeader(TcpClient &client, TCPRequest &request, uint64_t userId,
                   uint64_t roleId, std::string endpoint, uint64_t payloadSize);

public:
  ClientCommunicator(std::string serverAddress, uint16_t serverPort);
  void sendRequest(uint64_t userId, uint64_t roleId, std::string endpoint,
                   std::vector<unsigned char> &payload);
  std::pair<uint32_t, std::vector<unsigned char>> receiveResponse();
  ~ClientCommunicator() = default;
};