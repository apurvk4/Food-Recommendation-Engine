#pragma once

#include "ProtocolDefinitions.h"

#include "ProtocolParser.h"
#include <cstdint>

class ResponseParser : public ProtocolParser {
  uint64_t responseHeaderBytesRead;
  bool responseHeaderParsed;
  ResponseHeader parsedResponseHeader;
  ResponseHeader parseResponseHeader();

public:
  ResponseParser(const std::vector<unsigned char> &buffer);
  U64 getSessionId();
  verbs getVerb();
  U64 getPayloadSize();
  SString getEndpoint();
  U16 getResponseCode();
  U64 getResponsePayloadSize();
  std::vector<unsigned char> getResponsePayload();
};
