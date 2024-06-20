#include "Client/FoodRecommendationClient.h"
#include "Client/TcpClient.h"
#include "FoodItem.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"
#include "Sockets/TcpSocket.h"
#include "User.h"
#include <iostream>
#include <unordered_map>

void FoodRecommendationClient::loginUser() {
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
    roleId = 2;
    break;
  case 2:
    roleId = 1;
    break;
  case 3:
    roleId = 3;
    break;
  default:
    std::cerr << "Invalid choice\n";
    return;
  }
  TcpClient client("127.0.0.1", 1234);
  if (!client.connectToServer()) {
    std::cerr << "Failed to connect to server\n";
    return;
  }
  int localPort = client.getLocalPort();
  std::cout << "localPort : " << localPort << "\n";
  TCPRequest request;
  std::string ip{"127.0.0.1"};
  request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.senderPort = localPort;
  request.protocolHeader.senderPort = localPort;
  request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.receiverPort = 1234;
  request.protocolHeader.requestId = 1;
  loginData loginData;
  loginData.userId = userId;
  loginData.password = password;
  loginData.roleId = roleId;
  request.protocolHeader.payloadSize = loginData.getSize();
  std::vector<unsigned char> requestBuffer;
  writeProtocolHeader(requestBuffer, request.protocolHeader);
  auto loginDataBuffer = loginData.serialize();
  requestBuffer.insert(requestBuffer.end(), loginDataBuffer.begin(),
                       loginDataBuffer.end());
  client.sendData(requestBuffer);
  std::vector<unsigned char> data = client.receiveData();
  ProtocolParser parser{data};
  uint32_t reqId = parser.getRequestId();
  if (reqId == 0) {
    DTO::User user{0, std::string(""), std::string(""), 0, 0};
    user.deserialize(parser.getPayload());
    if (activeUser != nullptr) {
      delete activeUser;
    }
    activeUser = new DTO::User(user);
    std::cout << "Name : " << (std::string)user.name << std::endl;
    std::cout << "userId : " << user.userId << std::endl;
    if (user.roleId == (U64)1) {
      std::cout << "Role : Admin\n";
    } else if (user.roleId == (U64)2) {
      std::cout << "Role : Employee\n";
    } else if (user.roleId == (U64)3) {
      std::cout << "Role : Chef\n";
    }
  } else {
    std::cout << "Login failed\n";
    SString message;
    message.deserialize(parser.getPayload());
    std::cout << "Error : " << (std::string)message << std::endl;
  }
}

void FoodRecommendationClient::logout() {
  if (activeUser != nullptr) {
    delete activeUser;
    activeUser = nullptr;
  }
}

void FoodRecommendationClient::addFoodItem() {
  if (activeUser == nullptr) {
    std::cerr << "Please login first\n";
    return;
  }
  TcpClient client("127.0.0.1", 1234);
  if (!client.connectToServer()) {
    std::cerr << "Failed to connect to server\n";
    return;
  }
  int localPort = client.getLocalPort();
  std::cout << "localPort : " << localPort << "\n";
  TCPRequest request;
  std::string ip{"127.0.0.1"};
  request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.senderPort = localPort;
  request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.receiverPort = 1234;
  request.protocolHeader.requestId = 3;
  std::cout << "Enter Food Item Name : ";
  std::string itemName;
  std::cin >> itemName;
  std::cout << "Enter Food Item Price : ";
  double price;
  std::cin >> price;
  std::unordered_map<int, std::string> foodItemTypes = {
      {1, "Breakfast"}, {2, "Lunch"}, {3, "Dinner"}};
  std::cout << "press 1. For Breakfast\n2. For Lunch\n3. For Dinner\n";
  int choice;
  U64 foodItemTypeId = 0;
  std::cin >> choice;
  switch (choice) {
  case 1:
    foodItemTypeId = 1;
    break;
  case 2:
    foodItemTypeId = 2;
    break;
  case 3:
    foodItemTypeId = 3;
    break;
  default:
    std::cerr << "Invalid choice\n";
    return;
  }
  DTO::FoodItem foodItem{0, itemName, price, true, foodItemTypeId};
  request.protocolHeader.payloadSize = foodItem.getSize();
  std::vector<unsigned char> requestBuffer;
  writeProtocolHeader(requestBuffer, request.protocolHeader);
  Pair<U64, DTO::FoodItem> foodItemPair;
  foodItemPair.first = activeUser->userId;
  foodItemPair.second = foodItem;
  auto foodItemBuffer = foodItemPair.serialize();
  requestBuffer.insert(requestBuffer.end(), foodItemBuffer.begin(),
                       foodItemBuffer.end());
  client.sendData(requestBuffer);
  std::vector<unsigned char> data = client.receiveData();
  ProtocolParser parser{data};
  uint32_t reqId = parser.getRequestId();
  if (reqId == 0) {
    std::cout << "Food Item added successfully\n";
  } else {
    std::cout << "Failed to add food item\n";
    SString message;
    message.deserialize(parser.getPayload());
    std::cout << "Error : " << (std::string)message << std::endl;
  }
}

void FoodRecommendationClient::getFoodItems() {
  if (activeUser == nullptr) {
    std::cerr << "Please login first\n";
    return;
  }
  TcpClient client("127.0.0.1", 1234);
  if (!client.connectToServer()) {
    std::cerr << "Failed to connect to server\n";
    return;
  }
  int localPort = client.getLocalPort();
  std::cout << "localPort : " << localPort << "\n";
  TCPRequest request;
  std::string ip{"127.0.0.1"};
  request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.senderPort = localPort;
  request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
  request.protocolHeader.receiverPort = 1234;
  request.protocolHeader.requestId = 2;
  UserData sendData;
  sendData.userId = activeUser->userId;
  request.protocolHeader.payloadSize = sendData.getSize();
  std::vector<unsigned char> requestBuffer;
  writeProtocolHeader(requestBuffer, request.protocolHeader);
  auto dataBuffer = sendData.serialize();
  requestBuffer.insert(requestBuffer.end(), dataBuffer.begin(),
                       dataBuffer.end());
  client.sendData(requestBuffer);
  std::vector<unsigned char> data = client.receiveData();
  ProtocolParser parser{data};
  uint32_t reqId = parser.getRequestId();
  if (reqId == 0) {
    std::cout << "Food Items\n";
    std::vector<unsigned char> payload = parser.getPayload();
    std::cout << " received payload size : " << payload.size() << "\n";
    Array<DTO::FoodItem> foodItems;
    foodItems.deserialize(payload);
    std::vector<DTO::FoodItem> foodItems1;
    for (int i = 0; i < foodItems.NumberOfItems(); i++) {
      foodItems1.push_back(foodItems[i]);
    }
    for (auto &foodItem : foodItems1) {
      std::cout << "Name : " << (std::string)foodItem.itemName << std::endl;
      std::cout << "Price : " << foodItem.price << std::endl;
      std::cout << "Quantity : " << foodItem.foodItemTypeId << std::endl;
      std::cout << "-----------------------------------\n";
    }
  } else {
    std::cout << "Failed to get food items\n";
    SString message;
    message.deserialize(parser.getPayload());
    std::cout << "Error : " << (std::string)message << std::endl;
  }
}

int FoodRecommendationClient::getCurrentRole() {
  if (activeUser == nullptr) {
    return -1;
  }
  return activeUser->roleId;
}