#pragma once

#include "Controller/IController.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include <memory>
#include <unordered_map>

using Controller::IController;

class RouteHandler {
  std::unordered_map<std::string, std::shared_ptr<IController>>
      registeredControllers;
  void defaultHandler(TcpSocket socket, TCPRequest request,
                      std::vector<unsigned char> &payload);

public:
  void registerController(std::shared_ptr<IController> controller);
  void routeRequest(TcpSocket socket, TCPRequest request,
                    std::vector<unsigned char> &payload);
};