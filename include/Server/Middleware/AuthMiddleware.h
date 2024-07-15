#pragma once

#include "IMiddleware.h"
#include "Server/Service/UserService.h"

namespace Middleware {

using Service::UserService;

class AuthMiddleware : public IMiddleware {
  std::shared_ptr<UserService> userService;
  uint64_t targetRoleId;

public:
  AuthMiddleware(std::shared_ptr<UserService> userService,
                 uint64_t targetRoleId);
  bool handleRequest(std::shared_ptr<TcpSocket> socket, TCPRequest request,
                     std::vector<unsigned char> &payload) override;
  ~AuthMiddleware() = default;
};

}; // namespace Middleware