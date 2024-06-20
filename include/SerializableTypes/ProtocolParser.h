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
  U32 getRequestId();
  U64 getPayloadSize();
  std::vector<unsigned char> getPayload();
};