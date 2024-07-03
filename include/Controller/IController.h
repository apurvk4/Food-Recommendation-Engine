#pragma once

#include "Middleware/IMiddleware.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "Sockets/TcpSocket.h"
#include <memory>

namespace Controller {

using Middleware::IMiddleware;

class IController {
public:
  virtual bool handleRequest(TcpSocket socket, TCPRequest &request,
                             std::vector<unsigned char> &payload) = 0;
  virtual std::string getEndpoint() = 0;
  virtual void registerPreprocessorMiddleware(std::shared_ptr<IMiddleware>) = 0;
  virtual void
      registerPostprocessorMiddleware(std::shared_ptr<IMiddleware>) = 0;
  virtual ~IController() = default;
};
}; // namespace Controller