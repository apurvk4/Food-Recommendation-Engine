#include "RequestParser.h"
#include <cstdint>

RequestParser::RequestParser(const std::vector<unsigned char> &buffer)
    : ProtocolParser(buffer), requestHeaderParsed(false),
      requestHeaderBytesRead(0) {}

U64 RequestParser::getSessionId() {
  if (!requestHeaderParsed) {
    parsedRequestHeader = parseRequestHeader();
    requestHeaderParsed = true;
  }
  return parsedRequestHeader.sessionId;
}

verbs RequestParser::getVerb() {
  if (!requestHeaderParsed) {
    parsedRequestHeader = parseRequestHeader();
    requestHeaderParsed = true;
  }
  return static_cast<verbs>((uint8_t)parsedRequestHeader.verb);
}

U64 RequestParser::getPayloadSize() {
  if (!requestHeaderParsed) {
    parsedRequestHeader = parseRequestHeader();
    requestHeaderParsed = true;
  }
  return parsedRequestHeader.payloadSize;
}

SString RequestParser::getEndpoint() {
  if (!requestHeaderParsed) {
    parsedRequestHeader = parseRequestHeader();
    requestHeaderParsed = true;
  }
  return parsedRequestHeader.endpoint;
}

std::vector<unsigned char> RequestParser::getPayload() {
  if (!requestHeaderParsed) {
    parsedRequestHeader = parseRequestHeader();
    requestHeaderParsed = true;
  }
  return safeSubVector(buffer, requestHeaderBytesRead,
                       requestHeaderBytesRead +
                           parsedRequestHeader.payloadSize);
}

RequestHeader RequestParser::parseRequestHeader() {
  RequestHeader header;
  parsedHeader = parseHeader();
  if (parsedHeader.messageType != (U8)MessageType::REQUEST) {
    throw std::runtime_error("Invalid message type");
  }
  uint64_t bytesRead = protocolHeaderBytesRead;
  uint64_t dataLeft = buffer.size() - bytesRead;
  if (dataLeft < parsedHeader.messageHeaderSize) {
    throw std::out_of_range("Not enough data to parse request header");
  }
  bytesRead += header.sessionId.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));

  bytesRead += header.verb.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint8_t)));

  bytesRead += header.payloadSize.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));

  size_t endpointSize = (uint32_t)parsedHeader.messageHeaderSize -
                        (bytesRead - protocolHeaderBytesRead);

  bytesRead += header.endpoint.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + endpointSize));

  if (bytesRead - protocolHeaderBytesRead != parsedHeader.messageHeaderSize) {
    throw std::out_of_range(
        "Invalid message header size defined in protocol header");
  }
  requestHeaderBytesRead = bytesRead;
  return header;
}
