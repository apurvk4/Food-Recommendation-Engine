#pragma once

#include "Client/UserHandler.h"
#include "FoodItem.h"
#include "IApplication.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "User.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
class ClientApplication : public IApplication {
  std::unique_ptr<UserHandler> userHandler;

  void displayMenu();
  void handleUserSelection(int choice);

public:
  ClientApplication();
  void start() override;
  void stop() override;
};