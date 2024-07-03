#pragma once

#include "IController.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"
#include "UserService.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace Controller {

using Middleware::IMiddleware;

class AuthController : public IController {
  std::vector<std::shared_ptr<IMiddleware>> m_preprocessors;
  std::vector<std::shared_ptr<IMiddleware>> m_postprocessors;
  std::shared_ptr<Service::UserService> m_userService;
  std::string m_baseAuthEndpoint;
  std::unordered_map<
      std::string,
      std::function<bool(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload)>>
      m_authRoutes;

  bool login(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
             std::vector<unsigned char> &payload);
  bool logout(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload);

public:
  AuthController(const std::string &authEndpoint,
                 std::shared_ptr<Service::UserService> userService);
  bool handleRequest(TcpSocket socket, TCPRequest &request,
                     std::vector<unsigned char> &payload) override;
  std::string getEndpoint() override;
  void registerPreprocessorMiddleware(
      std::shared_ptr<IMiddleware> middleware) override;
  void registerPostprocessorMiddleware(
      std::shared_ptr<IMiddleware> middleware) override;
};
}; // namespace Controller