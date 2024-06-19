#pragma once

#include "SString.h"
#include "SerializableTypes.h"

#pragma pack(1)
struct ProtocolHeader {
  U8 protocolVersion;
  U32 senderIp;
  U16 senderPort;
  U32 receiverIp;
  U16 receiverPort;
  U8 messageType;
  U32 messageHeaderSize;
};

enum MessageType : uint8_t {
  REQUEST = 0,
  RESPONSE = 1,
};

struct RequestHeader {
  U64 userId;
  U8 verb;
  U64 payloadSize;
  SString endpoint;
};

struct ResponseHeader {
  U64 userId;
  U8 verb;
  U64 payloadSize;
  U16 responseCode;
  SString endpoint;
};

enum verbs : uint8_t {
  GET = 0,
  POST = 1,
  PUT = 2,
};

struct TCPRequest {
  ProtocolHeader protocolHeader;
  RequestHeader requestHeader;
  unsigned char *payload;
};

struct TCPResponse {
  ProtocolHeader protocolHeader;
  ResponseHeader responseHeader;
  unsigned char *payload;
};

#pragma pop()