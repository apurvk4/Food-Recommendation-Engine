#include "SerializableTypes/ProtocolParser.h"
#include <cstdint>
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

std::vector<unsigned char> ProtocolParser::getPayload() {
  if (!protocolHeaderParsed) {
    parsedHeader = parseHeader();
    protocolHeaderParsed = true;
  }
  // total payload size
  std::cout << "payload size: " << parsedHeader.payloadSize << "\n";
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
  protocolHeaderBytesRead = bytesRead;
  return header;
}