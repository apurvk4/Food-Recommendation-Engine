#include "Client/ClientCommunicator.h"
#include "Client/UserHandler.h"
#include "Config.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"
#include "Sockets/Socket.h"

ClientCommunicator::ClientCommunicator(std::string serverAddress,
                                       uint16_t serverPort)
    : client(serverAddress, serverPort), serverAddress(serverAddress),
      serverPort(serverPort) {
  client.connectToServer();
}
void ClientCommunicator::writeHeader(TcpClient &client, TCPRequest &request,
                                     uint64_t userId, uint64_t roleId,
                                     std::string endpoint,
                                     uint64_t payloadSize) {
  request.protocolHeader.userId = userId;
  request.protocolHeader.roleId = roleId;
  request.protocolHeader.payloadSize = payloadSize;
  request.protocolHeader.receiverIp =
      Socket::convertIpAddress(serverAddress).s_addr;
  request.protocolHeader.receiverPort = serverPort;
  uint16_t localPort = client.getLocalPort();
  std::string ip{SERVER_IP};
  request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.senderPort = localPort;
  strncpy(request.protocolHeader.endpoint, endpoint.c_str(), MAX_ENDPOINT_SIZE);
  request.protocolHeader.requestId = 0;
}

void ClientCommunicator::sendRequest(uint64_t userId, uint64_t roleId,
                                     std::string endpoint,
                                     std::vector<unsigned char> &payload) {

  TCPRequest request;
  writeHeader(client, request, userId, roleId, endpoint, payload.size());
  std::vector<unsigned char> buffer;
  writeProtocolHeader(buffer, request.protocolHeader);
  buffer.insert(buffer.end(), payload.begin(), payload.end());
  client.sendData(buffer);
}

std::pair<uint32_t, std::vector<unsigned char>>
ClientCommunicator::receiveResponse() {
  std::vector<unsigned char> buffer = client.receiveData();
  ProtocolParser parser{buffer};
  uint32_t requestId = parser.getRequestId();
  auto payload = parser.getPayload();
  return {requestId, payload};
}