#include "SerializableTypes/ProtocolParser.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include <cstdint>
#include <cstring>
#include <iostream>

ProtocolParser::ProtocolParser(const std::vector<unsigned char> &buffer)
    : buffer(buffer), protocolHeaderParsed(false), protocolHeaderBytesRead(0) {}

std::vector<unsigned char>
ProtocolParser::safeSubVector(const std::vector<unsigned char> &buffer,
                              size_t start, size_t end) {
  // Ensure end does not exceed buffer size
  end = std::min(end, buffer.size());
  if (start > end) {
    throw std::out_of_range("Start index is greater than end index");
  }
  return std::vector<unsigned char>(buffer.begin() + start,
                                    buffer.begin() + end);
}

U32 ProtocolParser::getSenderIp() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.senderIp;
}

U16 ProtocolParser::getSenderPort() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.senderPort;
}

U32 ProtocolParser::getReceiverIp() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.receiverIp;
}

U16 ProtocolParser::getReceiverPort() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.receiverPort;
}

U64 ProtocolParser::getUserId() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.userId;
}

U64 ProtocolParser::getRoleId() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.roleId;
}

U64 ProtocolParser::getPayloadSize() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.payloadSize;
}

U32 ProtocolParser::getRequestId() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return parsedHeader.requestId;
}

std::string ProtocolParser::getEndpoint() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  std::cout << "request endpoint : "
            << std::string(parsedHeader.endpoint, MAX_ENDPOINT_SIZE) << "\n";
  return std::string(parsedHeader.endpoint, MAX_ENDPOINT_SIZE);
}

std::vector<unsigned char> ProtocolParser::getPayload() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  return safeSubVector(buffer, protocolHeaderBytesRead, buffer.size());
}

ProtocolHeader ProtocolParser::parseHeader() {
  ProtocolHeader header;
  uint64_t bytesRead = 0;
  bytesRead = header.senderIp.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint32_t)));
  bytesRead += header.senderPort.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint16_t)));
  bytesRead += header.receiverIp.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint32_t)));
  bytesRead += header.receiverPort.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint16_t)));
  bytesRead += header.requestId.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint32_t)));
  bytesRead += header.payloadSize.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));
  bytesRead += header.userId.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));
  bytesRead += header.roleId.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));

  std::vector<unsigned char> endpointData =
      safeSubVector(buffer, bytesRead, bytesRead + MAX_ENDPOINT_SIZE);
  if (endpointData.size() < MAX_ENDPOINT_SIZE) {
    throw std::out_of_range("Insufficient data for endpoint in header");
  }
  std::memcpy(header.endpoint, endpointData.data(), MAX_ENDPOINT_SIZE);
  protocolHeaderBytesRead = bytesRead + MAX_ENDPOINT_SIZE;
  return header;
}