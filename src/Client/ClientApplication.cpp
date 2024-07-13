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

ClientApplication::ClientApplication() : userHandler(nullptr) {}

void ClientApplication::start() {
  while (true) {
    displayMenu();
    InputHandler inputHandler;
    int choice = inputHandler.getInput<int>(
        [](const int &input) { return input >= 1 && input <= 3; });
    handleUserSelection(choice);
    userHandler.reset();
    char ch;
    std::cout << "Do you want to continue? (y/n): ";
    std::cin >> ch;
    if (ch != 'y') {
      break;
    }
  }
}

void ClientApplication::stop() {
  if (userHandler) {
    userHandler->logout();
  }
  exit(0);
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
  try {
    userHandler->performAction();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}