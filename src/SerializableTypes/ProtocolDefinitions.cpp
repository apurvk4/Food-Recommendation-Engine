#include "SerializableTypes/ProtocolDefinitions.h"

void writeProtocolHeader(std::vector<unsigned char> &buffer,
                         ProtocolHeader &header) {
  auto result = header.senderIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.senderPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.statusCode.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.payloadSize.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.userId.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.roleId.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = std::vector<unsigned char>(header.endpoint,
                                      header.endpoint + MAX_ENDPOINT_SIZE);
  std::string endpoint(result.begin(), result.end());
  buffer.insert(buffer.end(), result.begin(), result.end());
}

void writeResponse(std::vector<unsigned char> &buffer, TCPRequest &response,
                   U32 responseCode, std::vector<unsigned char> &payload) {
  ProtocolHeader header;
  header.senderIp = response.protocolHeader.receiverIp;
  header.senderPort = response.protocolHeader.receiverPort;
  header.receiverIp = response.protocolHeader.senderIp;
  header.receiverPort = response.protocolHeader.senderPort;
  header.statusCode = responseCode;
  header.payloadSize = payload.size();
  std::memcpy(header.endpoint, response.protocolHeader.endpoint,
              MAX_ENDPOINT_SIZE);
  writeProtocolHeader(buffer, header);
  buffer.insert(buffer.end(), payload.begin(), payload.end());
}