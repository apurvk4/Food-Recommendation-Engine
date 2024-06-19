#include "Server/ResponseParser.h"
#include <cstdint>

ResponseParser::ResponseParser(const std::vector<unsigned char> &buffer)
    : ProtocolParser(buffer), responseHeaderBytesRead(0),
      responseHeaderParsed(false) {}

U64 ResponseParser::getSessionId() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return parsedResponseHeader.userId;
}

verbs ResponseParser::getVerb() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return static_cast<verbs>((uint8_t)parsedResponseHeader.verb);
}

U64 ResponseParser::getPayloadSize() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return parsedResponseHeader.payloadSize;
}

SString ResponseParser::getEndpoint() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return parsedResponseHeader.endpoint;
}

U16 ResponseParser::getResponseCode() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return parsedResponseHeader.responseCode;
}

U64 ResponseParser::getResponsePayloadSize() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return parsedResponseHeader.payloadSize;
}

std::vector<unsigned char> ResponseParser::getResponsePayload() {
  if (!responseHeaderParsed) {
    parsedResponseHeader = parseResponseHeader();
    responseHeaderParsed = true;
  }
  return std::vector<unsigned char>(buffer.begin() + responseHeaderBytesRead,
                                    buffer.end());
}

ResponseHeader ResponseParser::parseResponseHeader() {
  ResponseHeader header;
  parsedHeader = parseHeader();
  if (parsedHeader.messageType != (U8)MessageType::RESPONSE) {
    throw std::runtime_error("Invalid message type");
  }
  uint64_t bytesRead = protocolHeaderBytesRead;
  uint64_t dataLeft = buffer.size() - bytesRead;

  if (dataLeft < parsedHeader.messageHeaderSize) {
    throw std::out_of_range("Not enough data to parse response header");
  }

  bytesRead += header.userId.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));

  bytesRead += header.verb.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint8_t)));

  bytesRead += header.payloadSize.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint64_t)));

  bytesRead += header.responseCode.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + sizeof(uint16_t)));

  size_t endpointSize = (uint32_t)parsedHeader.messageHeaderSize -
                        (bytesRead - protocolHeaderBytesRead);

  bytesRead += header.endpoint.deserialize(
      safeSubVector(buffer, bytesRead, bytesRead + endpointSize));

  if ((bytesRead - protocolHeaderBytesRead) != parsedHeader.messageHeaderSize) {
    throw std::out_of_range("Invalid message header size in response header");
  }

  responseHeaderBytesRead = bytesRead;
  return header;
}
