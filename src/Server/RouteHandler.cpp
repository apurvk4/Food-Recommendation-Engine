#include "Server/RouteHandler.h"
#include "Controller/IController.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "Sockets/SocketUtils.h"
#include <cstring>
#include <regex>

void RouteHandler::registerController(std::shared_ptr<IController> controller) {
  registeredControllers[controller->getEndpoint()] = controller;
}

void RouteHandler::routeRequest(TcpSocket socket, TCPRequest request,
                                std::vector<unsigned char> &payload) {
  std::smatch match;
  std::regex pattern(R"(^(/[^/]+)(/.*)?$)");
  std::string endpoint = request.protocolHeader.endpoint;
  if (std::regex_match(endpoint, match, pattern)) {
    std::cout << "Matched: " << match[1] << std::endl;
    std::string controllerKey = match[1].str();
    auto controller = registeredControllers.find(controllerKey);
    if (controller != registeredControllers.end()) {
      if (controller->second->handleRequest(std::move(socket), request,
                                            payload)) {
        return;
      }
    }
  }
  defaultHandler(std::move(socket), request, payload);
}

void RouteHandler::defaultHandler(TcpSocket socket, TCPRequest request,
                                  std::vector<unsigned char> &payload) {
  std::cout << "Default handler called" << std::endl;
  ProtocolHeader header;
  header.senderIp = request.protocolHeader.receiverIp;
  header.senderPort = request.protocolHeader.receiverPort;
  header.receiverIp = request.protocolHeader.senderIp;
  header.receiverPort = request.protocolHeader.senderPort;
  header.requestId = 400;
  header.payloadSize = 0;
  std::memcpy(header.endpoint, request.protocolHeader.endpoint,
              MAX_ENDPOINT_SIZE);
  SString response("No controller found for endpoint: " +
                   std::string(header.endpoint));
  header.payloadSize = response.getSize();
  std::vector<unsigned char> buffer;
  writeProtocolHeader(buffer, header);
  auto result = response.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  socket.sendData(buffer);
  socket.shutdown(SHUTDOWN_BOTH);
  socket.close();
}
