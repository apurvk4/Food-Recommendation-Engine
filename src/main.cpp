#include "EmployeeService.h"
#include "FeedbackDAO.h"
#include "MenuDAO.h"
#include "ProtocolDefinitions.h"
#include "RequestParser.h"
#include "ResponseParser.h"
#include "SString.h"
#include "UserDAO.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 4096

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

void writeResponseHeader(std::vector<unsigned char> &responseBuffer,
                         ResponseHeader &responseHeader) {
  auto result = responseHeader.sessionId.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.verb.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.payloadSize.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.responseCode.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
  result = responseHeader.endpoint.serialize();
  responseBuffer.insert(responseBuffer.end(), result.begin(), result.end());
}

size_t getResponseHeaderSize(ResponseHeader &header) {
  return header.payloadSize.getSize() + header.responseCode.getSize() +
         header.sessionId.getSize() + header.verb.getSize() +
         header.endpoint.getSize();
}

int server() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Binding the socket to the address and port specified
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Accepting an incoming connection
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Reading data from the client
  std::vector<unsigned char> data;
  int valread = 0;
  do {
    valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
      std::cout << "number of bytes read : " << valread << "\n";
      data.insert(data.end(), buffer, buffer + valread);
    }
  } while (valread > 0);
  std::cout << "number of bytes read ds : " << data.size() << "\n";
  RequestParser requestParser(data);

  std::cout << "Session ID: " << requestParser.getSessionId() << std::endl;
  std::cout << "Verb: " << static_cast<int>(requestParser.getVerb())
            << std::endl;
  std::cout << "Payload Size: " << requestParser.getPayloadSize() << std::endl;
  std::cout << "Endpoint: " << (std::string)requestParser.getEndpoint()
            << std::endl;
  SString value;
  value.deserialize(requestParser.getPayload());

  std::cout << "payload val : " << (std::string)value << "\n";

  // Constructing a response
  TCPResponse response;
  response.protocolHeader.protocolVersion = 1;
  response.protocolHeader.senderIp = address.sin_addr.s_addr;
  response.protocolHeader.senderPort = ntohs(address.sin_port);
  response.protocolHeader.receiverIp = requestParser.getSenderIp();
  response.protocolHeader.receiverPort = requestParser.getSenderPort();
  response.protocolHeader.messageType = RESPONSE;

  response.responseHeader.sessionId = requestParser.getSessionId();
  response.responseHeader.verb = requestParser.getVerb();
  response.responseHeader.payloadSize = 0; // No payload for the response
  response.responseHeader.responseCode = 200;
  response.responseHeader.endpoint = requestParser.getEndpoint();

  response.protocolHeader.messageHeaderSize =
      getResponseHeaderSize(response.responseHeader);

  std::vector<unsigned char> responseBuffer;
  writeProtocolHeader(responseBuffer, response.protocolHeader);
  writeResponseHeader(responseBuffer, response.responseHeader);
  //   std::memcpy(responseBuffer.data(), &(response.protocolHeader),
  //               sizeof(response.protocolHeader));
  //   std::memcpy(responseBuffer.data() + sizeof(response.protocolHeader),
  //               &response.responseHeader, sizeof(response.responseHeader));

  // Sending response to the client
  int valsend = 0;
  do {
    valsend = send(new_socket, responseBuffer.data(), responseBuffer.size(), 0);
    if (valsend > 0) {
      responseBuffer.erase(responseBuffer.begin(),
                           responseBuffer.begin() + valsend);
    }
  } while (valsend > 0 && responseBuffer.size() > 0);
  std::cout << "number of bytes sent : " << valsend << "\n";
  // Closing the socket
  close(new_socket);
  close(server_fd);
  return 0;
}

size_t getRequestHeaderSize(RequestHeader &header) {
  std::cout << (std::string)header.endpoint
            << " : endpoint, endpoint size : " << header.endpoint.getSize()
            << std::endl;
  std::cout << "total request header size : "
            << header.sessionId.getSize() + header.verb.getSize() +
                   header.payloadSize.getSize() + header.endpoint.getSize()
            << "\n";
  return header.sessionId.getSize() + header.verb.getSize() +
         header.payloadSize.getSize() + header.endpoint.getSize();
}

void writeRequestHeader(std::vector<unsigned char> &requestBuffer,
                        RequestHeader &requestHeader) {
  auto result = requestHeader.sessionId.serialize();
  requestBuffer.insert(requestBuffer.end(), result.begin(), result.end());
  result = requestHeader.verb.serialize();
  requestBuffer.insert(requestBuffer.end(), result.begin(), result.end());
  result = requestHeader.payloadSize.serialize();
  requestBuffer.insert(requestBuffer.end(), result.begin(), result.end());
  result = requestHeader.endpoint.serialize();
  requestBuffer.insert(requestBuffer.end(), result.begin(), result.end());
}

int client() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};

  // Creating socket file descriptor
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error\n";
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported\n";
    return -1;
  }

  // Connecting to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Connection Failed\n";
    return -1;
  }

  // Constructing a request
  TCPRequest request;
  request.protocolHeader.protocolVersion = 1;
  request.protocolHeader.senderIp = serv_addr.sin_addr.s_addr;
  request.protocolHeader.senderPort = ntohs(serv_addr.sin_port);
  request.protocolHeader.receiverIp = serv_addr.sin_addr.s_addr;
  request.protocolHeader.receiverPort = ntohs(serv_addr.sin_port);
  request.protocolHeader.messageType = REQUEST;

  request.requestHeader.sessionId = 12345;
  request.requestHeader.verb = GET;
  SString value = std::string("hello");
  request.requestHeader.payloadSize =
      value.getSize(); // No payload for this example
  request.requestHeader.endpoint = std::string("/login");

  request.protocolHeader.messageHeaderSize =
      getRequestHeaderSize(request.requestHeader);

  std::vector<unsigned char> requestBuffer;
  writeProtocolHeader(requestBuffer, request.protocolHeader);
  writeRequestHeader(requestBuffer, request.requestHeader);
  auto valueSer = value.serialize();
  requestBuffer.insert(requestBuffer.end(), valueSer.begin(), valueSer.end());
  // Sending request to the server
  int valsend = 0;
  do {
    valsend = send(sock, requestBuffer.data(), requestBuffer.size(), 0);
    if (valsend > 0) {
      requestBuffer.erase(requestBuffer.begin(),
                          requestBuffer.begin() + valsend);
    }
  } while (valsend > 0 && requestBuffer.size() > 0);
  shutdown(sock, SHUT_WR);
  std::cout << "number of bytes sent : " << valsend << "\n";
  // Reading response from the server
  int valread = 0;
  std::vector<unsigned char> data;
  do {
    valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
      std::cout << "number of bytes read : " << valread << "\n";
      data.insert(data.end(), buffer, buffer + valread);
    }
  } while (valread > 0);
  ResponseParser responseParser(data);

  std::cout << "Session ID: " << responseParser.getSessionId() << std::endl;
  std::cout << "Verb: " << static_cast<int>(responseParser.getVerb())
            << std::endl;
  std::cout << "Payload Size: " << responseParser.getPayloadSize() << std::endl;
  std::cout << "Response Code: " << responseParser.getResponseCode()
            << std::endl;
  std::cout << "Endpoint: " << (std::string)responseParser.getEndpoint()
            << std::endl;

  // Closing the socket
  close(sock);
  return 0;
}

// void initDbConnection() {
//   DbConnection::initDbConnection("tcp://localhost:3306", "cppserver",
//                                  "cppserver", "FoodRecommendationSystem");
// }

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <server/client>\n";
    return -1;
  }
  if (std::string(argv[1]) == "server") {
    server();
  } else if (std::string(argv[1]) == "client") {
    client();
  } else {
    std::cerr << "Usage: " << argv[0] << " <server/client>\n";
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
