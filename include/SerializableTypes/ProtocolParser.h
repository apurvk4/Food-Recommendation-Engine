#pragma once

#include "SerializableTypes/ProtocolDefinitions.h"

class ProtocolParser {
  bool protocolHeaderParsed;

protected:
  std::vector<unsigned char>
  safeSubVector(const std::vector<unsigned char> &buffer, size_t start,
                size_t end);
  uint64_t protocolHeaderBytesRead;
  std::vector<unsigned char> buffer;
  ProtocolHeader parsedHeader;
  ProtocolHeader parseHeader();

public:
  ProtocolParser(const std::vector<unsigned char> &buffer);
  U32 getSenderIp();
  U16 getSenderPort();
  U32 getReceiverIp();
  U16 getReceiverPort();
  U32 getStatusCode();
  U64 getPayloadSize();
  U64 getUserId();
  U64 getRoleId();
  std::string getEndpoint();
  std::vector<unsigned char> getPayload();
};