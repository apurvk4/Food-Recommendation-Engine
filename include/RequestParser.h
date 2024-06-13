#pragma once

#include "ProtocolDefinitions.h"
#include "ProtocolParser.h"
#include "SString.h"

class RequestParser : public ProtocolParser {
  uint64_t requestHeaderBytesRead;
  bool requestHeaderParsed;
  RequestHeader parsedRequestHeader;
  RequestHeader parseRequestHeader();

public:
  RequestParser(const std::vector<unsigned char> &buffer);
  U64 getSessionId();
  verbs getVerb();
  U64 getPayloadSize();
  SString getEndpoint();
  std::vector<unsigned char> getPayload();
};