#include "Server/RequestHandler.h"
#include "AdminController.h"
#include "ChefService.h"
#include "DAO/ILoginDAO.h"
#include "DAO/INotificationDAO.h"
#include "DAO/LoginDAO.h"
#include "DAO/NotificationDAO.h"
#include "FoodItem.h"
#include "RoleDAO.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"
#include "Service/LoginService.h"
#include "Service/UserService.h"
#include "Sockets/SocketUtils.h"
#include "UserService.h"
#include <exception>
#include <memory>

using Server::RequestHandler;

std::pair<TCPRequest, std::vector<unsigned char>>
RequestHandler::parseHeaders(std::vector<unsigned char> &data) {
  ProtocolParser parser{data};
  TCPRequest request;
  request.protocolHeader.senderIp = parser.getSenderIp();
  request.protocolHeader.senderPort = parser.getSenderPort();
  request.protocolHeader.receiverIp = parser.getReceiverIp();
  request.protocolHeader.receiverPort = parser.getReceiverPort();
  request.protocolHeader.requestId = parser.getRequestId();
  request.protocolHeader.payloadSize = parser.getPayloadSize();
  std::cout << "parsed headers\n";
  return std::make_pair(request, parser.getPayload());
}

void RequestHandler::writeProtocolHeader(std::vector<unsigned char> &buffer,
                                         ProtocolHeader &header) {
  auto result = header.senderIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.senderPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverIp.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.receiverPort.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.requestId.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
  result = header.payloadSize.serialize();
  buffer.insert(buffer.end(), result.begin(), result.end());
}

void RequestHandler::handleRequest(TcpSocket socket) {
  try {
    auto data = socket.receiveData();
    auto [request, payload] = parseHeaders(data);
    std::cout << "handling request\n";
    handleRequest(std::move(socket), request.protocolHeader.requestId, request,
                  payload);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void RequestHandler::handleLoginRequest(TcpSocket socket, TCPRequest request,
                                        std::vector<unsigned char> payload) {
  loginData loginData;
  loginData.deserialize(payload);
  std::cout << "login data desrialized\n";
  std::shared_ptr<DAO::IUserDAO> userDAO = std::make_shared<DAO::UserDAO>();
  std::shared_ptr<DAO::ILoginDAO> loginDAO = std::make_shared<DAO::LoginDAO>();
  service::LoginService loginService{userDAO, loginDAO};
  std::cout << "Service starting\n";
  try {
    DTO::Login login{0, loginData.userId, loginData.roleId, SString{""}};
    auto user = loginService.login(login);
    std::cout << "sql done\n";
    ProtocolHeader header;
    header.senderIp = request.protocolHeader.senderIp;
    header.senderPort = request.protocolHeader.senderPort;
    header.receiverIp = request.protocolHeader.receiverIp;
    header.receiverPort = request.protocolHeader.receiverPort;
    if ((std::string)user.password == (std::string)loginData.password) {
      auto data = user.serialize();
      header.requestId = 0;
      header.payloadSize = data.size();
      std::vector<unsigned char> buffer;
      writeProtocolHeader(buffer, header);
      buffer.insert(buffer.end(), data.begin(), data.end());
      socket.sendData(buffer);
      socket.shutdown(SHUTDOWN_BOTH);
      socket.close();
    } else {
      // send failure response
      header.requestId = (uint32_t)400;
      SString message{"invalid user or password"};
      std::vector<unsigned char> messageData = message.serialize();
      header.payloadSize = messageData.size();
      std::vector<unsigned char> buffer;
      writeProtocolHeader(buffer, header);
      buffer.insert(buffer.end(), messageData.begin(), messageData.end());
      socket.sendData(buffer);
      socket.shutdown(SHUTDOWN_BOTH);
      socket.close();
    }
  } catch (std::exception &e) {
    // send failure response
    ProtocolHeader header;
    header.senderIp = request.protocolHeader.senderIp;
    header.senderPort = request.protocolHeader.senderPort;
    header.receiverIp = request.protocolHeader.receiverIp;
    header.receiverPort = request.protocolHeader.receiverPort;
    header.requestId = 400;
    SString message{"invalid user or password"};
    std::vector<unsigned char> messageData = message.serialize();
    header.payloadSize = messageData.size();
    std::vector<unsigned char> buffer;
    writeProtocolHeader(buffer, header);
    buffer.insert(buffer.end(), messageData.begin(), messageData.end());
    socket.sendData(buffer);
    std::cout << "send done\n";
    socket.shutdown(SHUT_WR);
    socket.close();
  }
}

void RequestHandler::handleGetFoodItemsRequest(
    TcpSocket socket, TCPRequest request, std::vector<unsigned char> payload) {
  UserData userData;
  userData.deserialize(payload);
  Controller::AdminController adminController;
  auto foodItems = adminController.getFoodItems(userData.userId);
  ProtocolHeader header;
  header.senderIp = request.protocolHeader.senderIp;
  header.senderPort = request.protocolHeader.senderPort;
  header.receiverIp = request.protocolHeader.receiverIp;
  header.receiverPort = request.protocolHeader.receiverPort;
  header.requestId = 0;
  header.payloadSize = foodItems.getSize();
  std::cout << "handle Get Food Request payload size : " << header.payloadSize
            << "\n";
  std::vector<unsigned char> buffer;
  writeProtocolHeader(buffer, header);
  auto foodItemSerialized = foodItems.serialize();
  buffer.insert(buffer.end(), foodItemSerialized.begin(),
                foodItemSerialized.end());
  socket.sendData(buffer);
  socket.shutdown(SHUTDOWN_BOTH);
  socket.close();
}

void RequestHandler::handleAddFoodItemRequest(
    TcpSocket socket, TCPRequest request, std::vector<unsigned char> payload) {
  Pair<U64, DTO::FoodItem> data;
  data.deserialize(payload);
  Controller::AdminController adminController;
  adminController.addFoodItem(data.first, data.second);
  ProtocolHeader header;
  header.senderIp = request.protocolHeader.senderIp;
  header.senderPort = request.protocolHeader.senderPort;
  header.receiverIp = request.protocolHeader.receiverIp;
  header.receiverPort = request.protocolHeader.receiverPort;
  header.requestId = 0;
  header.payloadSize = 0;
  std::vector<unsigned char> buffer;
  writeProtocolHeader(buffer, header);
  socket.sendData(buffer);
  socket.shutdown(SHUTDOWN_BOTH);
  socket.close();
}

void RequestHandler::handleRequest(TcpSocket socket, U32 requestId,
                                   TCPRequest request,
                                   std::vector<unsigned char> payload) {
  uint32_t reqId = requestId;
  switch (reqId) {
  case 1:
    handleLoginRequest(std::move(socket), request, payload);
    break;
  case 2:
    handleGetFoodItemsRequest(std::move(socket), request, payload);
    break;
  case 3:
    handleAddFoodItemRequest(std::move(socket), request, payload);
    break;
  default:
    break;
  }
}