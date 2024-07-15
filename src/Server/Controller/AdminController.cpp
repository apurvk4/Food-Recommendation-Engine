#include "Server/Controller/AdminController.h"
#include "Category.h"
#include "FoodItem.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "Sockets/SocketUtils.h"
#include <regex>

using Controller::AdminController;

AdminController::AdminController(
    const std::string &authEndpoint,
    std::shared_ptr<Service::UserService> userService,
    std::shared_ptr<Service::FoodItemService> foodItemService)
    : baseAuthEndpoint(authEndpoint), userService(userService),
      foodItemService(foodItemService) {
  authRoutes.insert(
      {"/addFoodItem",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->addFoodItem(socket, request, payload);
       }});
  authRoutes.insert(
      {"/removeFoodItem",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->removeFoodItem(socket, request, payload);
       }});
  authRoutes.insert(
      {"/updateFoodItem",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->updateFoodItem(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getFoodItems",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getFoodItems(socket, request, payload);
       }});
  authRoutes.insert(
      {"/addUser",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->addUser(socket, request, payload);
       }});
  authRoutes.insert(
      {"/viewNotifications",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewNotifications(socket, request, payload);
       }});
  authRoutes.insert(
      {"/addAttribute",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->addAttribute(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getAllAttributes",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getAllAttributes(socket, request, payload);
       }});
  authRoutes.insert(
      {"/viewAttributes",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewFoodItemAttributes(socket, request, payload);
       }});
  authRoutes.insert(
      {"/removeAttribute",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->removeAttribute(socket, request, payload);
       }});
};

bool AdminController::handleRequest(TcpSocket socket, TCPRequest &request,
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

std::string AdminController::getEndpoint() { return baseAuthEndpoint; }

void AdminController::registerPreprocessorMiddleware(
    std::shared_ptr<IMiddleware> middleware) {
  preprocessors.push_back(middleware);
}

void AdminController::registerPostprocessorMiddleware(
    std::shared_ptr<IMiddleware> middleware) {
  postprocessors.push_back(middleware);
}

bool AdminController::addFoodItem(std::shared_ptr<TcpSocket> socket,
                                  TCPRequest &request,
                                  std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    DTO::FoodItem foodItem;
    foodItem.deserialize(payload);
    if (foodItemService->addFoodItem(foodItem)) {
      std::vector<unsigned char> responsePayload;
      writeResponse(responseBuffer, request, 0, responsePayload);
    } else {
      SString responseString{"Failed to add food item"};
      std::vector<unsigned char> responsePayload = responseString.serialize();
      writeResponse(responseBuffer, request, 400, responsePayload);
    }
  } catch (std::exception &e) {
    std::cout << "Error adding food item: " << e.what() << std::endl;
    SString responseString{"Failed to add food item due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::removeFoodItem(std::shared_ptr<TcpSocket> socket,
                                     TCPRequest &request,
                                     std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    U64 foodItemId;
    foodItemId.deserialize(payload);
    if (foodItemService->deleteFoodItem(foodItemId)) {
      std::vector<unsigned char> responsePayload;
      writeResponse(responseBuffer, request, 0, responsePayload);
    } else {
      SString responseString{"Failed to remove food item"};
      std::vector<unsigned char> responsePayload = responseString.serialize();
      writeResponse(responseBuffer, request, 400, responsePayload);
    }
  } catch (std::exception &e) {
    std::cout << "Error removing food item: " << e.what() << std::endl;
    SString responseString{"Failed to remove food item due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::updateFoodItem(std::shared_ptr<TcpSocket> socket,
                                     TCPRequest &request,
                                     std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    DTO::FoodItem foodItem;
    foodItem.deserialize(payload);
    if (foodItemService->updateFoodItem(foodItem)) {
      std::vector<unsigned char> responsePayload;
      writeResponse(responseBuffer, request, 0, responsePayload);
    } else {
      SString responseString{"Failed to update food item"};
      std::vector<unsigned char> responsePayload = responseString.serialize();
      writeResponse(responseBuffer, request, 400, responsePayload);
    }
  } catch (std::exception &e) {
    std::cout << "Error updating food item: " << e.what() << std::endl;
    SString responseString{"Failed to update food item due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::getFoodItems(std::shared_ptr<TcpSocket> socket,
                                   TCPRequest &request,
                                   std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    U64 categoryId;
    categoryId.deserialize(payload);
    std::vector<DTO::FoodItem> foodItems =
        foodItemService->getFoodItemsByCategory(
            (DTO::Category)(uint64_t)categoryId);
    std::vector<unsigned char> responsePayload;
    Array<DTO::FoodItem> foodItemsArray{foodItems};
    responsePayload = foodItemsArray.serialize();
    writeResponse(responseBuffer, request, 0, responsePayload);
  } catch (std::exception &e) {
    std::cout << "Error getting food items: " << e.what() << std::endl;
    SString responseString{"Failed to get food items due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::addUser(std::shared_ptr<TcpSocket> socket,
                              TCPRequest &request,
                              std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    DTO::User user;
    user.deserialize(payload);
    if (userService->addUser(user)) {
      std::vector<unsigned char> responsePayload;
      writeResponse(responseBuffer, request, 0, responsePayload);
    } else {
      SString responseString{"Failed to add user"};
      std::vector<unsigned char> responsePayload = responseString.serialize();
      writeResponse(responseBuffer, request, 400, responsePayload);
    }
  } catch (std::exception &e) {
    std::cout << "Error adding user: " << e.what() << std::endl;
    SString responseString{"Failed to add user due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::viewNotifications(std::shared_ptr<TcpSocket> socket,
                                        TCPRequest &request,
                                        std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    U64 userId;
    userId.deserialize(payload);
    std::vector<DTO::Notification> notifications =
        userService->getUnreadNotifications(userId);
    std::vector<unsigned char> responsePayload;
    Array<DTO::Notification> notificationsArray{notifications};
    responsePayload = notificationsArray.serialize();
    writeResponse(responseBuffer, request, 0, responsePayload);
  } catch (std::exception &e) {
    std::cout << "Error getting notifications: " << e.what() << std::endl;
    SString responseString{"Failed to get notifications due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::addAttribute(std::shared_ptr<TcpSocket> socket,
                                   TCPRequest &request,
                                   std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    Pair<U64, U64> data;
    data.deserialize(payload);
    U64 foodItemId = data.first;
    U64 attributeId = data.second;
    foodItemService->addAttributeToFoodItem(foodItemId, attributeId);
    std::vector<unsigned char> responsePayload;
    writeResponse(responseBuffer, request, 0, responsePayload);
  } catch (std::exception &e) {
    std::cout << "Error adding attribute: " << e.what() << std::endl;
    SString responseString{"Failed to add attribute due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::getAllAttributes(std::shared_ptr<TcpSocket> socket,
                                       TCPRequest &request,
                                       std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    std::vector<std::pair<uint64_t, std::string>> attributes =
        foodItemService->getAllAttributes();
    Array<Pair<U64, SString>> attributesArray;
    for (auto &attribute : attributes) {
      attributesArray.push_back(
          Pair<U64, SString>{attribute.first, attribute.second});
    }
    auto payload = attributesArray.serialize();
    writeResponse(responseBuffer, request, 0, payload);
  } catch (std::exception &e) {
    std::cout << "Error getting attributes: " << e.what() << std::endl;
    SString responseString{"Failed to get attributes due to : " +
                           std::string(e.what())};
    auto payload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, payload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::viewFoodItemAttributes(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  std::cout << "running view FoodItem Attributes\n";
  std::vector<unsigned char> responseBuffer;
  try {
    U64 foodItemId;
    foodItemId.deserialize(payload);
    std::vector<std::pair<uint64_t, std::string>> attributes =
        foodItemService->getFoodItemAttributes(foodItemId);

    Array<Pair<U64, SString>> attributesArray;
    for (auto &attribute : attributes) {
      attributesArray.push_back(
          Pair<U64, SString>{attribute.first, attribute.second});
    }
    std::vector<unsigned char> payload = attributesArray.serialize();
    writeResponse(responseBuffer, request, 0, payload);
  } catch (std::exception &e) {
    std::cout << "Error getting food item attributes: " << e.what()
              << std::endl;
    SString responseString{"Failed to get food item attributes due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> payload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, payload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AdminController::removeAttribute(std::shared_ptr<TcpSocket> socket,
                                      TCPRequest &request,
                                      std::vector<unsigned char> &payload) {
  std::vector<unsigned char> responseBuffer;
  try {
    Pair<U64, U64> data;
    data.deserialize(payload);
    U64 foodItemId = data.first;
    U64 attributeId = data.second;
    foodItemService->removeAttributeFromFoodItem(foodItemId, attributeId);
    std::vector<unsigned char> responsePayload;
    writeResponse(responseBuffer, request, 0, responsePayload);
  } catch (std::exception &e) {
    std::cout << "Error removing attribute: " << e.what() << std::endl;
    SString responseString{"Failed to remove attribute due to : " +
                           std::string(e.what())};
    std::vector<unsigned char> responsePayload = responseString.serialize();
    writeResponse(responseBuffer, request, 400, responsePayload);
  }
  try {
    socket->sendData(responseBuffer);
  } catch (std::exception &e) {
    std::cout << "Error sending response: " << e.what() << std::endl;
    return false;
  }
  return true;
}