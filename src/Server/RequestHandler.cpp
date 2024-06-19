#include "Server/RequestHandler.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "Server/RequestParser.h"
#include "Sockets/SocketUtils.h"

using Server::RequestHandler;

void RequestHandler::registerEndpoint(std::string endpoint,
                                      HandlerFn function) {
  registeredEndpoints[endpoint] = function;
}

std::pair<TCPRequest, std::vector<unsigned char>>
RequestHandler::parseHeaders(std::vector<unsigned char> &data) {
  RequestParser requestParser{data};
  requestParser.getPayloadSize();
  TCPRequest request;
  request.protocolHeader.senderIp = requestParser.getSenderIp();
  request.protocolHeader.senderPort = requestParser.getSenderPort();
  request.protocolHeader.receiverIp = requestParser.getReceiverIp();
  request.protocolHeader.receiverPort = requestParser.getReceiverPort();
  request.protocolHeader.messageType = requestParser.getMessageType();
  request.protocolHeader.protocolVersion = requestParser.getProtocolVersion();
  request.protocolHeader.messageHeaderSize =
      requestParser.getMessageHeaderSize();
  request.requestHeader.verb = requestParser.getVerb();
  request.requestHeader.endpoint = requestParser.getEndpoint();
  request.requestHeader.userId = requestParser.getSessionId();
  request.requestHeader.payloadSize = requestParser.getPayloadSize();
  return std::make_pair(request, requestParser.getPayload());
}

void RequestHandler::writeProtocolHeader(std::vector<unsigned char> &buffer,
                                         ProtocolHeader &header) {
  auto result = header.protocolVersion.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.senderIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.senderPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.messageType.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.messageHeaderSize.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
}

void RequestHandler::writeResponseHeader(
    std::vector<unsigned char> &responseBuffer,
    ResponseHeader &responseHeader) {
  auto result = responseHeader.userId.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.verb.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.payloadSize.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.responseCode.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.endpoint.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
}

void RequestHandler::handleRequest(TcpSocket socket) {
  try {
    auto data = socket.receiveData();
    auto [request, payload] = parseHeaders(data);
    if (registeredEndpoints.find(request.requestHeader.endpoint) !=
        registeredEndpoints.end()) {
      auto response =
          registeredEndpoints[request.requestHeader.endpoint](payload, request);
      TCPResponse tcpResponse;
      tcpResponse.protocolHeader = request.protocolHeader;
      tcpResponse.responseHeader.userId = request.requestHeader.userId;
      tcpResponse.responseHeader.verb = request.requestHeader.verb;
      tcpResponse.responseHeader.endpoint = request.requestHeader.endpoint;
      if (response.first != ErrorStatus::E_NONE) {
        tcpResponse.responseHeader.responseCode = (uint16_t)response.first;
      } else {
        tcpResponse.responseHeader.responseCode = (uint16_t)ErrorStatus::E_NONE;
      }
      tcpResponse.responseHeader.payloadSize = response.second.size();
      tcpResponse.responseHeader.responseCode = (uint16_t)ErrorStatus::E_NONE;
      std::vector<unsigned char> buffer;
      writeProtocolHeader(buffer, tcpResponse.protocolHeader);
      writeResponseHeader(buffer, tcpResponse.responseHeader);
      buffer.insert(buffer.end(), response.second.begin(),
                    response.second.end());
      socket.sendData(buffer);
      socket.close();
    }
  } catch (std::exception &e) {
    socket.close();
    std::cerr << "Error while handling request : " << e.what();
    return;
  }
  socket.close();
  std::cerr << "Endpoint not registered" << std::endl;
}