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
  // Matches a string starting with a slash, followed by non-slash characters
  // (group 1), and optionally the rest starting from another slash (group 2)
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
  std::vector<unsigned char> response;
  SString responseString("No controller found for endpoint: " +
                         std::string(request.protocolHeader.endpoint));
  std::vector<unsigned char> responsePayload = responseString.serialize();
  writeResponse(response, request, 400, responsePayload);
  socket.sendData(response);
}
