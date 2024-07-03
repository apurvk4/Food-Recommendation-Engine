#include "Client/ClientApplication.h"
#include "Client/AdminHandler.h"
#include "Client/ChefHandler.h"
#include "Client/EmployeeHandler.h"
#include "Client/InputHandler.h"
#include "Client/TcpClient.h"
#include "DTO/Category.h"
#include "FoodItem.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/ProtocolParser.h"
#include "Sockets/TcpSocket.h"
#include "User.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

// void FoodRecommendationClient::loginUser() {
//   std::cout << "Press 1 to login as a Employee\n 2 to login as a Admin\n 3 to
//   "
//                "login as a Chef\n";
//   int choice = 0;
//   std::cin >> choice;
//   std::cout << "Enter userId : ";
//   uint64_t userId;
//   std::cin >> userId;
//   std::cout << "Enter password : ";
//   std::string password;
//   std::cin >> password;
//   int roleId = 0;
//   switch (choice) {
//   case 1:
//     roleId = 2;
//     break;
//   case 2:
//     roleId = 1;
//     break;
//   case 3:
//     roleId = 3;
//     break;
//   default:
//     std::cerr << "Invalid choice\n";
//     return;
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     return;
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 1;
//   request.protocolHeader.userId = userId;
//   request.protocolHeader.roleId = roleId;
//   std::string endpoint = "/login";
//   std::copy(endpoint.begin(), endpoint.end(),
//   request.protocolHeader.endpoint);
//   request.protocolHeader.endpoint[endpoint.size()] = '\0';
//   std::cout << "endpoint : " << request.protocolHeader.endpoint << "\n";
//   LoginData loginData;
//   loginData.userId = userId;
//   loginData.password = password;
//   loginData.roleId = roleId;
//   request.protocolHeader.payloadSize = loginData.getSize();
//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   auto loginDataBuffer = loginData.serialize();
//   requestBuffer.insert(requestBuffer.end(), loginDataBuffer.begin(),
//                        loginDataBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     DTO::User user{0, std::string(""), std::string(""), 0, 0};
//     user.deserialize(parser.getPayload());
//     if (activeUser != nullptr) {
//       delete activeUser;
//     }
//     activeUser = new DTO::User(user);
//     std::cout << "Name : " << (std::string)user.name << std::endl;
//     std::cout << "userId : " << user.userId << std::endl;
//     if (user.roleId == (U64)1) {
//       std::cout << "Role : Admin\n";
//     } else if (user.roleId == (U64)2) {
//       std::cout << "Role : Employee\n";
//     } else if (user.roleId == (U64)3) {
//       std::cout << "Role : Chef\n";
//     }
//   } else {
//     std::cout << "Login failed\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//   }
// }

// void FoodRecommendationClient::logout() {
//   if (activeUser != nullptr) {
//     delete activeUser;
//     activeUser = nullptr;
//   }
// }

// void FoodRecommendationClient::addFoodItem() {
//   if (activeUser == nullptr) {
//     std::cerr << "Please login first\n";
//     return;
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     return;
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 3;
//   std::cout << "Enter Food Item Name : ";
//   std::string itemName;
//   std::cin >> itemName;
//   std::cout << "Enter Food Item Price : ";
//   double price;
//   std::cin >> price;
//   std::unordered_map<int, std::string> foodItemTypes = {
//       {1, "Breakfast"}, {2, "Lunch"}, {3, "Dinner"}};
//   std::cout << "press 1. For Breakfast\n2. For Lunch\n3. For Dinner\n";
//   int choice;
//   U64 foodItemTypeId = 0;
//   std::cin >> choice;
//   switch (choice) {
//   case 1:
//     foodItemTypeId = 1;
//     break;
//   case 2:
//     foodItemTypeId = 2;
//     break;
//   case 3:
//     foodItemTypeId = 3;
//     break;
//   default:
//     std::cerr << "Invalid choice\n";
//     return;
//   }
//   DTO::FoodItem foodItem{0, price, true, foodItemTypeId, itemName};
//   request.protocolHeader.payloadSize = foodItem.getSize();
//   std::cout << "size : " << foodItem.getSize() << "\n";
//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   Pair<U64, DTO::FoodItem> foodItemPair;
//   foodItemPair.first = activeUser->userId;
//   foodItemPair.second = foodItem;
//   auto foodItemBuffer = foodItemPair.serialize();
//   request.protocolHeader.payloadSize = foodItemPair.getSize();
//   requestBuffer.insert(requestBuffer.end(), foodItemBuffer.begin(),
//                        foodItemBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     std::cout << "Food Item added successfully\n";
//   } else {
//     std::cout << "Failed to add food item\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//   }
// }

// std::vector<DTO::FoodItem> FoodRecommendationClient::getFoodItems() {
//   if (activeUser == nullptr) {
//     std::cerr << "Please login first\n";
//     throw std::runtime_error("Please login first");
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     throw std::runtime_error("Failed to connect to server");
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 2;
//   UserData sendData;
//   sendData.userId = activeUser->userId;
//   request.protocolHeader.payloadSize = sendData.getSize();
//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   auto dataBuffer = sendData.serialize();
//   requestBuffer.insert(requestBuffer.end(), dataBuffer.begin(),
//                        dataBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     std::vector<unsigned char> payload = parser.getPayload();
//     Array<DTO::FoodItem> foodItems;
//     foodItems.deserialize(payload);
//     std::vector<DTO::FoodItem> foodItems1;
//     for (int i = 0; i < foodItems.NumberOfItems(); i++) {
//       foodItems1.push_back(foodItems[i]);
//     }
//     return foodItems1;
//   } else {
//     std::cout << "Failed to get food items\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//     throw std::runtime_error("Failed to get food items");
//   }
// }

// void FoodRecommendationClient::showFoodItems() {
//   auto result = getFoodItems();
//   for (auto &foodItem : result) {
//     std::cout << "------------------------------\n";
//     std::cout << "Food Item Id : " << foodItem.foodItemId << "\n";
//     std::cout << "Food Item Name : " << (std::string)foodItem.itemName <<
//     "\n"; std::cout << "Price : " << foodItem.price << "\n"; std::cout <<
//     "Availability : " << foodItem.availabilityStatus << "\n"; std::cout <<
//     "Food Item Type Id : " << foodItem.foodItemTypeId << "\n"; std::cout <<
//     "------------------------------\n";
//   }
// }

// void FoodRecommendationClient::updateFoodItem() {
//   if (activeUser == nullptr) {
//     std::cerr << "Please login first\n";
//     return;
//   }
//   auto result = getFoodItems();
//   for (auto &foodItem : result) {
//     std::cout << "------------------------------\n";
//     std::cout << "Food Item Id : " << foodItem.foodItemId << "\n";
//     std::cout << "Food Item Name : " << (std::string)foodItem.itemName <<
//     "\n"; std::cout << "Price : " << foodItem.price << "\n"; std::cout <<
//     "Availability : " << foodItem.availabilityStatus << "\n"; std::cout <<
//     "Food Item Type Id : " << foodItem.foodItemTypeId << "\n"; std::cout <<
//     "------------------------------\n";
//   }
//   std::cout << "Enter the food item id to update : ";
//   uint64_t foodItemId;
//   std::cin >> foodItemId;
//   // find the foodItem with the given id
//   DTO::FoodItem foodItem;
//   bool found = false;
//   for (auto &item : result) {
//     if (item.foodItemId == (U64)foodItemId) {
//       foodItem = item;
//       found = true;
//       break;
//     }
//   }
//   if (!found) {
//     std::cerr << "Food Item not found\n";
//     return;
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     return;
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 4;
//   int choice = 0;
//   std::cout << "\nEnter 1 to update price\n2 to update availability\nEnter 3
//   "
//                "to update name\n";
//   std::cin >> choice;
//   bool availability;
//   double price;
//   std::string name;
//   switch (choice) {
//   case 1:
//     std::cout << "Enter new price : ";
//     std::cin >> price;
//     foodItem.price = price;
//     break;
//   case 2:
//     std::cout << "Enter new availability : ";
//     std::cin >> availability;
//     foodItem.availabilityStatus = availability;
//     break;
//   case 3:
//     std::cout << "Enter new name : ";
//     std::cin >> name;
//     foodItem.itemName = name;
//     break;
//   default:
//     std::cerr << "Invalid choice\n";
//     return;
//   }
//   request.protocolHeader.payloadSize = foodItem.getSize();
//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   Pair<U64, DTO::FoodItem> foodItemPair;
//   foodItemPair.first = activeUser->userId;
//   foodItemPair.second = foodItem;
//   auto foodItemBuffer = foodItemPair.serialize();
//   requestBuffer.insert(requestBuffer.end(), foodItemBuffer.begin(),
//                        foodItemBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     std::cout << "Food Item updated successfully\n";
//   } else {
//     std::cout << "Failed to update food item\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//   }
// }

// void FoodRecommendationClient::deleteFoodItem() {
//   if (activeUser == nullptr) {
//     std::cerr << "Please login first\n";
//     return;
//   }
//   auto result = getFoodItems();
//   for (auto &foodItem : result) {
//     std::cout << "------------------------------\n";
//     std::cout << "Food Item Id : " << foodItem.foodItemId << "\n";
//     std::cout << "Food Item Name : " << (std::string)foodItem.itemName <<
//     "\n"; std::cout << "Price : " << foodItem.price << "\n"; std::cout <<
//     "Availability : " << foodItem.availabilityStatus << "\n"; std::cout <<
//     "Food Item Type Id : " << foodItem.foodItemTypeId << "\n"; std::cout <<
//     "------------------------------\n";
//   }
//   std::cout << "Enter the food item id to delete : ";
//   uint64_t foodItemId;
//   std::cin >> foodItemId;
//   // find the foodItem with the given id
//   DTO::FoodItem foodItem;
//   bool found = false;
//   for (auto &item : result) {
//     if (item.foodItemId == (U64)foodItemId) {
//       foodItem = item;
//       found = true;
//       break;
//     }
//   }
//   if (!found) {
//     std::cerr << "Food Item not found\n";
//     return;
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     return;
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 5;
//   request.protocolHeader.payloadSize = foodItem.getSize();
//   std::vector<unsigned char> requestBuffer;
//   writeProtocolHeader(requestBuffer, request.protocolHeader);
//   Pair<U64, DTO::FoodItem> foodItemPair;
//   foodItemPair.first = activeUser->userId;
//   foodItemPair.second = foodItem;
//   request.protocolHeader.payloadSize = foodItemPair.getSize();
//   auto foodItemBuffer = foodItemPair.serialize();
//   requestBuffer.insert(requestBuffer.end(), foodItemBuffer.begin(),
//                        foodItemBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     std::cout << "Food Item deleted successfully\n";
//   } else {
//     std::cout << "Failed to delete food item\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//   }
// }

// int FoodRecommendationClient::getCurrentRole() {
//   if (activeUser == nullptr) {
//     return -1;
//   }
//   return activeUser->roleId;
// }

// void FoodRecommendationClient::showFoodItemRecommendation() {
//   if (activeUser == nullptr) {
//     std::cerr << "Please login first\n";
//     return;
//   }
//   TcpClient client("127.0.0.1", 1234);
//   if (!client.connectToServer()) {
//     std::cerr << "Failed to connect to server\n";
//     return;
//   }
//   int localPort = client.getLocalPort();
//   std::cout << "localPort : " << localPort << "\n";
//   TCPRequest request;
//   std::string ip{"127.0.0.1"};
//   request.protocolHeader.senderIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.senderPort = localPort;
//   request.protocolHeader.receiverIp = Socket::convertIpAddress(ip).s_addr;
//   request.protocolHeader.receiverPort = 1234;
//   request.protocolHeader.requestId = 6;
//   Pair<U64, DTO::FoodItemType> foodItemTypePair;
//   foodItemTypePair.first = activeUser->userId;
//   foodItemTypePair.second = DTO::FoodItemType(0, std::string(""));
//   int choice = 0;
//   std::cout << "\nPress 1 for Breakfast\n2 for Lunch\n3 for Dinner\n";
//   std::cin >> choice;
//   switch (choice) {
//   case 1:
//     foodItemTypePair.second.foodItemTypeId = 1;
//     foodItemTypePair.second.foodItemType = "Breakfast";
//     break;
//   case 2:
//     foodItemTypePair.second.foodItemTypeId = 2;
//     foodItemTypePair.second.foodItemType = "Lunch";
//     break;
//   case 3:
//     foodItemTypePair.second.foodItemTypeId = 3;
//     foodItemTypePair.second.foodItemType = "Dinner";
//     break;
//   default:
//     std::cerr << "Invalid choice\n";
//     return;
//   }
//   request.protocolHeader.payloadSize = foodItemTypePair.getSize();
//   std::vector<unsigned char> requestBuffer;
//   auto foodItemTypeBuffer = foodItemTypePair.serialize();
//   requestBuffer.insert(requestBuffer.end(), foodItemTypeBuffer.begin(),
//                        foodItemTypeBuffer.end());
//   client.sendData(requestBuffer);
//   std::vector<unsigned char> data = client.receiveData();
//   ProtocolParser parser{data};
//   uint32_t reqId = parser.getRequestId();
//   if (reqId == 0) {
//     std::cout << "Recommended Food Items\n";
//     std::vector<unsigned char> payload = parser.getPayload();
//     Array<DTO::FoodItem> foodItems;
//     foodItems.deserialize(payload);
//     for (int i = 0; i < foodItems.NumberOfItems(); i++) {
//       std::cout << "------------------------------\n";
//       std::cout << "Food Item Id : " << foodItems[i].foodItemId << "\n";
//       std::cout << "Food Item Name : " << (std::string)foodItems[i].itemName
//                 << "\n";
//       std::cout << "Price : " << foodItems[i].price << "\n";
//       std::cout << "Availability : " << foodItems[i].availabilityStatus <<
//       "\n"; std::cout << "Food Item Type Id : " <<
//       foodItems[i].foodItemTypeId
//                 << "\n";
//       std::cout << "------------------------------\n";
//     }
//   } else {
//     std::cout << "Failed to get recommended food items\n";
//     SString message;
//     message.deserialize(parser.getPayload());
//     std::cout << "Error : " << (std::string)message << std::endl;
//   }
// }

ClientApplication::ClientApplication() : userHandler(nullptr) {}

void ClientApplication::run() {
  while (true) {
    displayMenu();
    InputHandler inputHandler;
    int choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 3; });
    handleUserSelection(choice);
    char ch;
    std::cout << "Do you want to continue? (y/n): ";
    std::cin >> ch;
    if (ch != 'y') {
      break;
    }
  }
}

void ClientApplication::displayMenu() {
  std::cout << "Select user type:" << std::endl;
  std::cout << "1. Employee" << std::endl;
  std::cout << "2. Chef" << std::endl;
  std::cout << "3. Admin" << std::endl;
}

void ClientApplication::handleUserSelection(int choice) {
  userHandler.reset();
  switch (choice) {
  case 1:
    userHandler = std::make_unique<EmployeeHandler>();
    break;
  case 2:
    userHandler = std::make_unique<ChefHandler>();
    break;
  case 3:
    userHandler = std::make_unique<AdminHandler>();
    break;
  default:
    std::cerr << "Invalid choice\n";
    return;
  }
  userHandler->performAction();
}