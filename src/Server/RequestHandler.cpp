#include "Server/RequestHandler.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"

#include <exception>

using Server::RequestHandler;

std::pair<TCPRequest, std::vector<unsigned char>>
RequestHandler::parseHeaders(std::vector<unsigned char> &data) {
  ProtocolParser parser{data};
  TCPRequest request;
  request.protocolHeader.senderIp = parser.getSenderIp();
  request.protocolHeader.senderPort = parser.getSenderPort();
  request.protocolHeader.receiverIp = parser.getReceiverIp();
  request.protocolHeader.receiverPort = parser.getReceiverPort();
  request.protocolHeader.statusCode = parser.getStatusCode();
  request.protocolHeader.payloadSize = parser.getPayloadSize();
  request.protocolHeader.userId = parser.getUserId();
  request.protocolHeader.roleId = parser.getRoleId();
  std::string endpoint = parser.getEndpoint();
  int size = endpoint.size();
  if (size > MAX_ENDPOINT_SIZE - 1) {
    size = MAX_ENDPOINT_SIZE - 1;
  }
  std::memcpy(request.protocolHeader.endpoint, endpoint.c_str(), size);
  request.protocolHeader.endpoint[size] = '\0';
  return std::make_pair(request, parser.getPayload());
}

void RequestHandler::handleRequest(TcpSocket socket) {
  try {
    auto data = socket.receiveData();
    auto [request, payload] = parseHeaders(data);
    std::cout << "handling request\n";
    routeHandler->routeRequest(std::move(socket), request, payload);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}