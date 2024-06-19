#include "DAO/INotificationDAO.h"
#include "DAO/NotificationDAO.h"
#include "EmployeeService.h"
#include "FeedbackDAO.h"
#include "MenuDAO.h"
#include "RoleDAO.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include "Server/FoodRecommendationServer.h"
#include "Server/RequestHandler.h"
#include "Server/ResponseParser.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include "UserService.h"

#define PORT 8080

// int client() {
//   int sock = 0;
//   struct sockaddr_in serv_addr;
//   char buffer[BUFFER_SIZE] = {0};

//   // Creating socket file descriptor
//   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//     std::cerr << "Socket creation error\n";
//     return -1;
//   }

//   serv_addr.sin_family = AF_INET;
//   serv_addr.sin_port = htons(PORT);

//   // Convert IPv4 and IPv6 addresses from text to binary form
//   if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//     std::cerr << "Invalid address/ Address not supported\n";
//     return -1;
//   }

//   // Connecting to the server
//   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//     std::cerr << "Connection Failed\n";
//     return -1;
//   }

//   // Constructing a request
//   TCPRequest request;
//   request.protocolHeader.protocolVersion = 1;
//   request.protocolHeader.senderIp = serv_addr.sin_addr.s_addr;
//   request.protocolHeader.senderPort = ntohs(serv_addr.sin_port);
//   request.protocolHeader.receiverIp = serv_addr.sin_addr.s_addr;
//   request.protocolHeader.receiverPort = ntohs(serv_addr.sin_port);
//   request.protocolHeader.messageType = REQUEST;

//   request.requestHeader.sessionId = 12345;
//   request.requestHeader.verb = GET;
//   SString value = std::string("hello");
//   request.requestHeader.payloadSize =
//       value.getSize(); // No payload for this example
//   request.requestHeader.endpoint = std::string("/login");

//   request.protocolHeader.messageHeaderSize =
//       getRequestHeaderSize(request.requestHeader);

//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   writeRequestHeader(requestBuffer, request.requestHeader);
//   auto valueSer = value.serialize();
//   requestBuffer.insert(requestBuffer.end(), valueSer.begin(),
//   valueSer.end());
//   // Sending request to the server
//   int valsend = 0;
//   do {
//     valsend = send(sock, requestBuffer.data(), requestBuffer.size(), 0);
//     if (valsend > 0) {
//       requestBuffer.erase(requestBuffer.begin(),
//                           requestBuffer.begin() + valsend);
//     }
//   } while (valsend > 0 && requestBuffer.size() > 0);
//   shutdown(sock, SHUT_WR);
//   std::cout << "number of bytes sent : " << valsend << "\n";
//   // Reading response from the server
//   int valread = 0;
//   std::vector<unsigned char> data;
//   do {
//     valread = read(sock, buffer, BUFFER_SIZE);
//     if (valread > 0) {
//       std::cout << "number of bytes read : " << valread << "\n";
//       data.insert(data.end(), buffer, buffer + valread);
//     }
//   } while (valread > 0);
//   ResponseParser responseParser(data);

//   std::cout << "Session ID: " << responseParser.getSessionId() << std::endl;
//   std::cout << "Verb: " << static_cast<int>(responseParser.getVerb())
//             << std::endl;
//   std::cout << "Payload Size: " << responseParser.getPayloadSize() <<
//   std::endl; std::cout << "Response Code: " <<
//   responseParser.getResponseCode()
//             << std::endl;
//   std::cout << "Endpoint: " << (std::string)responseParser.getEndpoint()
//             << std::endl;

//   // Closing the socket
//   close(sock);
//   return 0;
// }

void initDbConnection() {
  DbConnection::initDbConnection("tcp://localhost:3306", "cppserver",
                                 "cppserver", "FoodRecommendationSystem");
}

struct LoginRequest : public Serializable {
  U64 userId;
  U64 roleId;
  SString password;
  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> bytes;
    std::vector<unsigned char> userIdBytes = userId.serialize();
    bytes.insert(bytes.end(), userIdBytes.begin(), userIdBytes.end());
    std::vector<unsigned char> roleIdBytes = roleId.serialize();
    bytes.insert(bytes.end(), roleIdBytes.begin(), roleIdBytes.end());
    std::vector<unsigned char> passwordBytes = password.serialize();
    bytes.insert(bytes.end(), passwordBytes.begin(), passwordBytes.end());
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) {
    uint64_t bytesRead = userId.deserialize({bytes.begin(), bytes.end()});
    bytesRead += roleId.deserialize({bytes.begin() + bytesRead, bytes.end()});
    bytesRead += password.deserialize({bytes.begin() + bytesRead, bytes.end()});
    return bytesRead;
  }
  virtual std::size_t getSize() {
    return userId.getSize() + roleId.getSize() + password.getSize();
  }
};

struct LoginResponse : public Serializable {
  U64 userId;
  U64 roleId;
  SString name;
  U64 lastNotificationId;
  std::vector<unsigned char> serialize() {
    std::vector<unsigned char> bytes;
    std::vector<unsigned char> userIdBytes = userId.serialize();
    bytes.insert(bytes.end(), userIdBytes.begin(), userIdBytes.end());
    std::vector<unsigned char> roleIdBytes = roleId.serialize();
    bytes.insert(bytes.end(), roleIdBytes.begin(), roleIdBytes.end());
    std::vector<unsigned char> nameBytes = name.serialize();
    bytes.insert(bytes.end(), nameBytes.begin(), nameBytes.end());
    std::vector<unsigned char> lastNotificationIdBytes =
        lastNotificationId.serialize();
    bytes.insert(bytes.end(), lastNotificationIdBytes.begin(),
                 lastNotificationIdBytes.end());
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) {
    uint64_t bytesRead = userId.deserialize({bytes.begin(), bytes.end()});
    bytesRead += roleId.deserialize({bytes.begin() + bytesRead, bytes.end()});
    bytesRead += name.deserialize({bytes.begin() + bytesRead, bytes.end()});
    bytesRead += lastNotificationId.deserialize(
        {bytes.begin() + bytesRead, bytes.end()});
    return bytesRead;
  }
  virtual std::size_t getSize() {
    return userId.getSize() + roleId.getSize() + name.getSize() +
           lastNotificationId.getSize();
  }
};

std::pair<ErrorStatus, std::vector<unsigned char>>
loginUser(std::vector<unsigned char> payload, TCPRequest request) {
  LoginRequest loginRequest;
  loginRequest.deserialize(payload);
  std::shared_ptr<DAO::IUserDAO> userDAO = std::make_shared<DAO::UserDAO>();
  std::shared_ptr<DAO::IRoleDAO> roleDAO = std::make_shared<DAO::RoleDAO>();
  std::shared_ptr<DAO::INotificationDAO> notificationDAO =
      std::make_shared<DAO::NotificationDAO>();
  Service::UserService userService{userDAO, roleDAO, notificationDAO};
  auto user = userService.getUserById(loginRequest.userId);
  if (user.roleId != loginRequest.roleId) {
    SString message = "Invalid role for the user with userId " +
                      std::to_string(loginRequest.userId);
    return std::make_pair(ErrorStatus::E_InvalidRole, message.serialize());
  } else if (user.password != (std::string)loginRequest.password) {
    SString message = "Invalid password for the user with userId " +
                      std::to_string(loginRequest.userId);
    return std::make_pair(ErrorStatus::E_InvalidDetails, message.serialize());
  }
  LoginResponse loginResponse;
  loginResponse.userId = user.userId;
  loginResponse.roleId = user.roleId;
  loginResponse.name = user.name;
  loginResponse.lastNotificationId = user.lastNotificationId;
  return std::make_pair(ErrorStatus::E_NONE, loginResponse.serialize());
}

void server() {
  Server::RequestHandler handler;
  std::cout << "Registering endpoints\n";
  handler.registerEndpoint("login", loginUser);
  std::cout << "Starting server s\n";
  Server::FoodRecommendationServer server{handler, 5};
  std::string ip{"127.0.0.1"};
  std::cout << "Server started at " << ip << ":1234\n";
  server.start(Socket::convertIpAddress(ip), 1234);
}

void writeProtocolHeader(std::vector<unsigned char> &buffer,
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

void writeRequestHeader(std::vector<unsigned char> &responseBuffer,
                        RequestHeader &responseHeader) {
  auto result = responseHeader.userId.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.verb.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.payloadSize.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.endpoint.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
}

void login() {
  std::cout << "Press 1 to login as a Employee\n 2 to login as a Admin\n 3 to "
               "login as a Chef\n";
  int choice = 0;
  std::cin >> choice;
  std::cout << "Enter userId : ";
  uint64_t userId;
  std::cin >> userId;
  std::cout << "Enter password : ";
  std::string password;
  std::cin >> password;
  int roleId = 0;
  switch (choice) {
  case 1:
    roleId = 1;
    break;
  case 2:
    roleId = 2;
    break;
  case 3:
    roleId = 3;
    break;
  default:
    std::cerr << "Invalid choice\n";
    return;
  }
  LoginRequest loginRequest;
  loginRequest.userId = userId;
  loginRequest.roleId = roleId;
  loginRequest.password = password;
  TcpSocket socket{"127.0.0.1", 1234};
  socket.connect();
  uint16_t localPort = socket.getLocalPort();
  TCPRequest request;
  std::string ip{"127.0.0.1"};
  request.protocolHeader.protocolVersion = 1;
  request.protocolHeader.senderPort = localPort;
  request.protocolHeader.receiverPort = 1234;
  request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.messageType = REQUEST;
  request.protocolHeader.messageHeaderSize =
      request.requestHeader.verb.getSize() +
      request.requestHeader.endpoint.getSize() +
      request.requestHeader.payloadSize.getSize() +
      request.requestHeader.userId.getSize();
  request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
  request.requestHeader.verb = GET;
  request.requestHeader.endpoint = std::string("/login");
  request.requestHeader.userId = 0;
  request.requestHeader.payloadSize = loginRequest.getSize();
  std::vector<unsigned char> requestBuffer;
  writeProtocolHeader(requestBuffer, request.protocolHeader);
  writeRequestHeader(requestBuffer, request.requestHeader);
  auto loginRequestBytes = loginRequest.serialize();
  requestBuffer.insert(requestBuffer.end(), loginRequestBytes.begin(),
                       loginRequestBytes.end());
  socket.sendData(requestBuffer);
  socket.shutdown(SHUTDOWN_WR);
  std::vector<unsigned char> data = socket.receiveData();
  ResponseParser responseParser(data);
  if ((uint16_t)responseParser.getResponseCode() ==
      (uint16_t)ErrorStatus::E_NONE) {
    LoginResponse loginResponse;
    loginResponse.deserialize(responseParser.getResponsePayload());
    std::cout << "Login successful\n";
    std::cout << "User Id : " << loginResponse.userId << std::endl;
    std::cout << "Role Id : " << loginResponse.roleId << std::endl;
    std::cout << "Name : " << (std::string)loginResponse.name << std::endl;
  } else {
    std::cout << "Login failed\n";
    SString message;
    message.deserialize(responseParser.getResponsePayload());
    std::cout << "Error : " << (std::string)message << std::endl;
  }
}

void client() { login(); }

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <server/client>\n";
    return -1;
  }
  std::string mode = argv[1];
  if (mode == "s") {
    std::cout << "Starting server\n";
    server();
  } else if (mode == "c") {
    client();
  } else {
    std::cerr << "Invalid mode\n";
    return -1;
  }

  // initDbConnection();
  // std::shared_ptr<IUserDAO> userDAO = std::make_shared<UserDAO>();
  // std::shared_ptr<IMenuDAO> menuDAO = std::make_shared<MenuDAO>();
  // std::shared_ptr<IFeedbackDAO> feedbackDAO =
  // std::make_shared<FeedbackDAO>();

  // EmployeeService employeeService(userDAO, menuDAO, feedbackDAO);
  // auto menu = employeeService.getTodaysMenu();
  // std::cout << menu.size() << std::endl;
  // for (auto &menuDTO : menu) {
  //   std::cout << menuDTO.date << menuDTO.foodItems.size() << std::endl;
  // }
  // employeeService.sendFeedback(1, 1, "Good", 5);

  return 0;
}
