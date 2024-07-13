#pragma once

#include "SString.h"
#include "SerializableTypes.h"
#include "SerializableTypes/Serializable.h"

#define MAX_ENDPOINT_SIZE 30

struct ProtocolHeader {
  U32 senderIp;
  U16 senderPort;
  U32 receiverIp;
  U16 receiverPort;
  U32 requestId;
  U64 payloadSize;
  U64 userId;
  U64 roleId;
  char endpoint[MAX_ENDPOINT_SIZE]; // max size 30 characters
};

struct TCPRequest {
  ProtocolHeader protocolHeader;
  unsigned char *payload;
};

struct TCPResponse {
  ProtocolHeader protocolHeader;
  unsigned char *payload;
};

void writeProtocolHeader(std::vector<unsigned char> &buffer,
                         ProtocolHeader &header);

void writeResponse(std::vector<unsigned char> &buffer, TCPRequest &response,
                   U32 responseCode, std::vector<unsigned char> &payload);