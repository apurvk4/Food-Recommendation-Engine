#include "Client/FoodRecommendationClient.h"
#include "DAO/INotificationDAO.h"
#include "DAO/NotificationDAO.h"
#include "DAO/ReviewDAO.h"
#include "EmployeeService.h"
#include "MenuDAO.h"
#include "RoleDAO.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "SerializableTypes/Serializable.h"
#include "SerializableTypes/U64.h"
#include "Server/FoodRecommendationServer.h"
#include "Server/RequestHandler.h"
#include "Sockets/SocketUtils.h"
#include "Sockets/TcpSocket.h"
#include "UserService.h"

void initDbConnection() {
  DbConnection::initDbConnection("tcp://localhost:3306", "cppserver",
                                 "cppserver", "FoodRecommendationSystem");
}

void server() {
  initDbConnection();
  Server::RequestHandler handler;
  std::cout << "Starting server s\n";
  Server::FoodRecommendationServer server{handler, 5};
  std::string ip{"127.0.0.1"};
  std::cout << "Server started at " << ip << ":1234\n";
  server.start(Socket::convertIpAddress(ip), 1234);
}

void adminMenu(FoodRecommendationClient &client) {
  do {
    std::cout << "1. Add Food Item\n";
    std::cout << "2. Update Food Item\n";
    std::cout << "3. Delete Food Item\n";
    std::cout << "4. View Food Items\n";
    std::cout << "6. Logout\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
      client.addFoodItem();
      std::cout << "Add Food Item\n";
      break;
    case 2:
      std::cout << "Update Food Item\n";
      client.updateFoodItem();
      break;
    case 3:
      std::cout << "Delete Food Item\n";
      client.deleteFoodItem();
      break;
    case 4:
      std::cout << "View Food Items\n";
      client.showFoodItems();
      break;
    case 6:
      return;
    default:
      std::cerr << "Invalid choice\n";
      break;
    }
  } while (true);
}

void chefMenu(FoodRecommendationClient &clients) {
  do {
    std::cout << "1. View Food Item Recommendations\n";
    std::cout << "3. Logout\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
      std::cout << "View Food Item Recommendations\n";
      clients.showFoodItemRecommendation();
      break;
    case 3:
      return;
    default:
      std::cerr << "Invalid choice\n";
      break;
    }
  } while (true);
}

void client() {
  FoodRecommendationClient client;
  while (true) {
    client.loginUser();
    int currentRole = client.getCurrentRole();
    if (currentRole == 1) {
      std::cout << "Admin\n";
      adminMenu(client);
    } else if (currentRole == 2) {
      std::cout << "Employee\n";
    } else if (currentRole == 3) {
      std::cout << "Chef\n";
      chefMenu(client);
    } else {
      std::cerr << "Invalid role\n";
      return;
    }
    std::cout << "press q to quit";
    char c;
    std::cin >> c;
    if (c == 'q') {
      break;
    }
  }
  int choice = 0;
  client.getCurrentRole();
}

int main(int argc, char const *argv[]) {
  std::string s1 = "hello";
  SString s = s1;
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
  return 0;
}