#include "Controller/AuthController.h"
#include "ActivityType.h"
#include "LoginData.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "Sockets/SocketUtils.h"
#include "UserService.h"
#include <cstdint>
#include <regex>

using Controller::AuthController;

AuthController::AuthController(
    const std::string &authEndpoint,
    std::shared_ptr<Service::UserService> userService)
    : baseAuthEndpoint(authEndpoint), userService(userService) {
  authRoutes.insert(
      {"/login",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->login(socket, request, payload);
       }});
  authRoutes.insert(
      {"/logout",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->logout(socket, request, payload);
       }});
}

bool AuthController::handleRequest(TcpSocket socket, TCPRequest &request,
                                   std::vector<unsigned char> &payload) {
  std::string endpoint = request.protocolHeader.endpoint;
  std::smatch match;
  // Matches a string starting with a slash, followed by non-slash characters
  // (group 1), and optionally the rest starting from another slash (group 2)
  std::regex pattern(R"(^(/[^/]+)(/.*)?$)");
  std::shared_ptr<TcpSocket> socketPtr =
      std::make_shared<TcpSocket>(std::move(socket));
  if (std::regex_match(endpoint, match, pattern)) {
    std::string controllerKey = match[2].str();
    if (authRoutes.find(controllerKey) != authRoutes.end()) {
      for (auto &middleware : preprocessors) {
        if (middleware->handleRequest(socketPtr, request, payload)) {
          return true;
        }
      }
      if (authRoutes[controllerKey](socketPtr, request, payload)) {
        return true;
      }
      for (auto &middleware : postprocessors) {
        if (middleware->handleRequest(socketPtr, request, payload)) {
          return true;
        }
      }
      return true;
    } else {
      std::cout << "No route found for: " << controllerKey << std::endl;
    }
  }
  return false;
}

std::string AuthController::getEndpoint() { return baseAuthEndpoint; }

void AuthController::registerPreprocessorMiddleware(
    std::shared_ptr<IMiddleware> middleware) {
  preprocessors.push_back(middleware);
}

void AuthController::registerPostprocessorMiddleware(
    std::shared_ptr<IMiddleware> middleware) {
  postprocessors.push_back(middleware);
}

bool AuthController::login(std::shared_ptr<TcpSocket> socket,
                           TCPRequest &request,
                           std::vector<unsigned char> &payload) {
  LoginData data;
  std::vector<unsigned char> buffer;
  try {
    data.deserialize(payload);
    auto user = userService->getUserById(data.userId);
    if (user.password == data.password && user.roleId == data.roleId) {
      userService->addActivity(user.userId, DTO::ActivityType::Login);
      user.password = "";
      auto result = user.serialize();
      writeResponse(buffer, request, 0, result);
    } else if (user.roleId != data.roleId) {
      SString response("Invalid roleId for user");
      auto result = response.serialize();
      writeResponse(buffer, request, 400, result);
    } else {
      SString response("Invalid password for user");
      auto result = response.serialize();
      writeResponse(buffer, request, 400, result);
    }
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    SString response("Error failed to login due to : " + std::string(e.what()));
    auto result = response.serialize();
    writeResponse(buffer, request, 400, result);
  }
  try {
    socket->sendData(buffer);
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AuthController::logout(std::shared_ptr<TcpSocket> socket,
                            TCPRequest &request,
                            std::vector<unsigned char> &payload) {
  std::vector<unsigned char> buffer;
  try {
    auto user = userService->getUserById(request.protocolHeader.userId);
    if (userService->addActivity(user.userId, DTO::ActivityType::Logout)) {
      SString response("User logged out");
      auto result = response.serialize();
      writeResponse(buffer, request, 0, result);
    } else {
      SString response("Failed to logout user");
      auto result = response.serialize();
      writeResponse(buffer, request, 400, result);
    }
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    SString response("Error failed to logout due to : " +
                     std::string(e.what()));
    auto result = response.serialize();
    writeResponse(buffer, request, 400, result);
  }
  try {
    socket->sendData(buffer);
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}
