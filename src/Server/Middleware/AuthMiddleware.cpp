#include "Server/Middleware/AuthMiddleware.h"
#include "SerializableTypes/ProtocolDefinitions.h"

using Middleware::AuthMiddleware;

AuthMiddleware::AuthMiddleware(std::shared_ptr<UserService> userService,
                               uint64_t targetRoleId)
    : userService(userService), targetRoleId(targetRoleId) {}

bool AuthMiddleware::handleRequest(std::shared_ptr<TcpSocket> socket,
                                   TCPRequest request,
                                   std::vector<unsigned char> &payload) {
  std::cout << "running auth middleware\n";
  std::vector<unsigned char> buffer;
  bool requestHandled = false;
  try {
    auto user = userService->getUserById(request.protocolHeader.userId);
    if ((uint64_t)user.roleId != targetRoleId) {
      SString response = {"Error : unauthorized "};
      auto responseSer = response.serialize();
      writeResponse(buffer, request, 400, responseSer);
      requestHandled = true;
    } else {
      requestHandled = false;
    }
  } catch (const std::exception &e) {
    SString response = {"Error : " + std::string(e.what())};
    auto responseSer = response.serialize();
    writeResponse(buffer, request, 400, responseSer);
    requestHandled = true;
  }
  try {
    if (requestHandled) {
      socket->sendData(buffer);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in sending data to socket" << std::endl;
    requestHandled = true;
  }
  return requestHandled;
}