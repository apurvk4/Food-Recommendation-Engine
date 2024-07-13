#include "Controller/EmployeeController.h"
#include "Category.h"
#include "DiscardFeedbackQuestion.h"
#include "Feedback.h"
#include "FoodItem.h"
#include "SerializableTypes/Pair.h"
#include "SerializableTypes/SString.h"
#include <exception>
#include <regex>

using Controller::EmployeeController;

EmployeeController::EmployeeController(
    const std::string &authEndpoint,
    std::shared_ptr<Service::UserService> userService,
    std::shared_ptr<Service::FoodItemService> foodItemService,
    std::shared_ptr<Service::MenuService> menuService,
    std::shared_ptr<Service::RecommendationService> recommendationService)
    : userService(userService), foodItemService(foodItemService),
      menuService(menuService), recommendationService(recommendationService),
      baseAuthEndpoint(authEndpoint) {
  authRoutes.insert(
      {"/viewMenu",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewMenu(socket, request, payload);
       }});
  authRoutes.insert(
      {"/viewMenuRollout",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewMenuRollout(socket, request, payload);
       }});
  authRoutes.insert(
      {"/sendFeedback",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->sendFeedback(socket, request, payload);
       }});
  authRoutes.insert(
      {"/viewNotifications",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewNotifications(socket, request, payload);
       }});
  authRoutes.insert(
      {"/writeReview",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->writeReview(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getAttributes",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getAttributes(socket, request, payload);
       }});
  authRoutes.insert(
      {"/viewFoodItemAttributes",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->viewFoodItemAttributes(socket, request, payload);
       }});
  authRoutes.insert(
      {"/addFoodPreferences",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->addFoodPreferences(socket, request, payload);
       }});
  authRoutes.insert(
      {"/deletePreferences",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->deleteFoodPreferences(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getFoodPreferences",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getFoodPreferences(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getDiscardedItem",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getDiscardedFoodItems(socket, request, payload);
       }});
  authRoutes.insert(
      {"/getQuestions",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->getDiscardedFoodItemQuestions(socket, request, payload);
       }});
  authRoutes.insert(
      {"/addAnswer",
       [this](std::shared_ptr<TcpSocket> socket, TCPRequest &request,
              std::vector<unsigned char> &payload) -> bool {
         return this->addDiscardedFoodItemAnswer(socket, request, payload);
       }});
}

bool EmployeeController::handleRequest(TcpSocket socket, TCPRequest &request,
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
    }
  }
  return false;
}

std::string EmployeeController::getEndpoint() { return baseAuthEndpoint; }

void EmployeeController::registerPreprocessorMiddleware(
    std::shared_ptr<Middleware::IMiddleware> middleware) {
  preprocessors.push_back(middleware);
}

void EmployeeController::registerPostprocessorMiddleware(
    std::shared_ptr<Middleware::IMiddleware> middleware) {
  postprocessors.push_back(middleware);
}

bool EmployeeController::viewMenu(std::shared_ptr<TcpSocket> socket,
                                  TCPRequest &request,
                                  std::vector<unsigned char> &payload) {
  Pair<U64, SString> data;
  std::vector<unsigned char> response;
  try {
    data.deserialize(payload);
    auto date = data.second;
    DTO::Category category = (DTO::Category)(uint64_t)data.first;
    auto menus = menuService->getMenuByCategory(category, date);
    std::vector<std::pair<DTO::Menu, std::vector<DTO::FoodItem>>> result;
    for (auto &menu : menus) {
      std::vector<DTO::FoodItem> foodItems;
      for (auto &menuItem : menu.second) {
        auto foodItem = foodItemService->getFoodItemById(menuItem.foodItemId);
        foodItems.push_back(foodItem);
      }
      sortFoodItems(request.protocolHeader.userId, foodItems);
      result.push_back(std::make_pair(menu.first, foodItems));
    }
    if (result.size() == 0) {
      SString error("No menu found for the given date");
      std::cout << (std::string)error << std::endl;
      std::vector<unsigned char> serialized = error.serialize();
      writeResponse(response, request, 400, serialized);
      socket->sendData(response);
      return true;
    }
    Pair<DTO::Menu, Array<Pair<DTO::FoodItem, Double>>> responsePair;
    auto menu = result[0];
    responsePair.first = menu.first;
    for (auto &foodItem : menu.second) {
      responsePair.second.push_back(Pair<DTO::FoodItem, Double>(
          foodItem,
          recommendationService->getFoodItemRating(foodItem.foodItemId)));
    }
    std::vector<unsigned char> serialized = responsePair.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::viewMenuRollout(std::shared_ptr<TcpSocket> socket,
                                         TCPRequest &request,
                                         std::vector<unsigned char> &payload) {
  Pair<U64, SString> data;
  try {
    Pair<DTO::Menu, Array<Pair<Double, DTO::FoodItem>>> responsePayload;
    data.deserialize(payload);
    auto date = data.second;
    DTO::Category category = (DTO::Category)(uint64_t)data.first;
    auto menus = menuService->getMenuByCategory(category, date);
    if (menus.size() > 0) {
      std::pair<DTO::Menu, std::vector<DTO::MenuItem>> menuDetails = menus[0];
      std::vector<DTO::FoodItem> foodItems;
      for (auto &menuItem : menuDetails.second) {
        auto foodItem = foodItemService->getFoodItemById(menuItem.foodItemId);
        foodItems.push_back(foodItem);
      }
      sortFoodItems(request.protocolHeader.userId, foodItems);
      Array<Pair<Double, DTO::FoodItem>> foodItemArray;
      for (auto &foodItem : foodItems) {
        foodItemArray.push_back(
            Pair(Double(recommendationService->getFoodItemRating(
                     foodItem.foodItemId)),
                 foodItem));
      }
      responsePayload.first = menuDetails.first;
      responsePayload.second = foodItemArray;
    }
    std::vector<unsigned char> serialized = responsePayload.serialize();
    std::vector<unsigned char> response;
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    std::vector<unsigned char> response;
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::sendFeedback(std::shared_ptr<TcpSocket> socket,
                                      TCPRequest &request,
                                      std::vector<unsigned char> &payload) {
  DTO::Feedback feedback;
  try {
    feedback.deserialize(payload);
    foodItemService->addFeedback(feedback);
    std::vector<unsigned char> response;
    auto serialized = std::vector<unsigned char>();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    std::vector<unsigned char> response;
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::viewNotifications(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  U64 userId;
  std::vector<unsigned char> response;
  try {
    userId.deserialize(payload);
    auto notifications = userService->getUnreadNotifications(userId);
    Array<DTO::Notification> responseArray(notifications);
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::writeReview(std::shared_ptr<TcpSocket> socket,
                                     TCPRequest &request,
                                     std::vector<unsigned char> &payload) {
  DTO::Review review;
  try {
    review.deserialize(payload);
    foodItemService->addReview(review);
    std::vector<unsigned char> response;
    auto serialized = std::vector<unsigned char>();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    std::vector<unsigned char> response;
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::getAttributes(std::shared_ptr<TcpSocket> socket,
                                       TCPRequest &request,
                                       std::vector<unsigned char> &payload) {
  std::vector<unsigned char> response;
  try {
    auto attributes = foodItemService->getAllAttributes();
    Array<Pair<U64, SString>> responseArray;
    for (auto &attribute : attributes) {
      responseArray.push_back(
          Pair<U64, SString>(attribute.first, attribute.second));
    }
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::viewFoodItemAttributes(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  U64 foodItemId;
  std::vector<unsigned char> response;
  try {
    foodItemId.deserialize(payload);
    auto attributes = foodItemService->getFoodItemAttributes(foodItemId);
    Array<Pair<U64, SString>> responseArray;
    for (auto &attribute : attributes) {
      responseArray.push_back(
          Pair<U64, SString>(attribute.first, attribute.second));
    }
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::addFoodPreferences(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  Array<U64> data;
  std::vector<unsigned char> response;
  try {
    data.deserialize(payload);
    auto userId = request.protocolHeader.userId;
    std::vector<uint64_t> attributeIds;
    for (int i = 0; i < data.NumberOfItems(); i++) {
      attributeIds.push_back(data[i]);
    }
    userService->addUserFoodPreference(userId, attributeIds);
    std::vector<unsigned char> serialized = std::vector<unsigned char>();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::deleteFoodPreferences(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  std::cout << "delete food preferences called\n";
  Array<U64> data;
  std::vector<unsigned char> response;
  try {
    data.deserialize(payload);
    auto userId = request.protocolHeader.userId;
    std::vector<uint64_t> attributeIds;
    for (int i = 0; i < data.NumberOfItems(); i++) {
      attributeIds.push_back(data[i]);
    }
    userService->deleteUserFoodPreference(userId, attributeIds);
    std::vector<unsigned char> serialized = std::vector<unsigned char>();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::getFoodPreferences(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  U64 userId;
  std::vector<unsigned char> response;
  try {
    userId.deserialize(payload);
    auto attributes = userService->getUserFoodPreference(userId);
    auto attributeDetails = foodItemService->getAttributeDetails(attributes);
    Array<Pair<U64, SString>> responseArray;
    for (auto &attribute : attributeDetails) {
      responseArray.push_back(
          Pair<U64, SString>(attribute.first, attribute.second));
    }
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

void EmployeeController::sortFoodItems(uint64_t userId,
                                       std::vector<DTO::FoodItem> &foodItems) {
  std::vector<std::pair<uint64_t, std::string>> userAttributes;
  try {
    auto preferences = userService->getUserFoodPreference(userId);
    userAttributes = foodItemService->getAttributeDetails(preferences);
  } catch (const std::exception &e) {
    std::cout << "Failed to get the user Food Preferences :  " << e.what()
              << std::endl;
    return;
  }

  auto hasMatchingAttribute = [&userAttributes,
                               this](const DTO::FoodItem &foodItem) {
    auto foodItemAttributes =
        foodItemService->getFoodItemAttributes(foodItem.foodItemId);
    for (const auto &attr : foodItemAttributes) {
      for (const auto &userAttr : userAttributes) {
        if (userAttr.first == attr.first) {
          return true;
        }
      }
    }
    return false;
  };

  std::stable_sort(
      foodItems.begin(), foodItems.end(),
      [&hasMatchingAttribute](const DTO::FoodItem &a, const DTO::FoodItem &b) {
        return hasMatchingAttribute(a) && !hasMatchingAttribute(b);
      });
}

bool EmployeeController::getDiscardedFoodItems(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  std::vector<unsigned char> response;
  try {
    auto foodItems = foodItemService->getDiscardedFoodItems();
    Array<DTO::FoodItem> responseArray;
    for (auto &foodItem : foodItems) {
      responseArray.push_back(foodItem);
    }
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::getDiscardedFoodItemQuestions(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  U64 foodItemId;
  std::vector<unsigned char> response;
  try {
    foodItemId.deserialize(payload);
    auto questions = foodItemService->getQuestionByFoodItemId(foodItemId);
    Array<DTO::DiscardFeedbackQuestion> responseArray;
    for (auto &question : questions) {
      responseArray.push_back(question);
    }
    std::vector<unsigned char> serialized = responseArray.serialize();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}

bool EmployeeController::addDiscardedFoodItemAnswer(
    std::shared_ptr<TcpSocket> socket, TCPRequest &request,
    std::vector<unsigned char> &payload) {
  DTO::DiscardFeedbackAnswer answer;
  std::vector<unsigned char> response;
  try {
    answer.deserialize(payload);
    foodItemService->addDiscardFeedbackAnswer(answer.answer, answer.userId,
                                              answer.questionId);
    std::vector<unsigned char> serialized = std::vector<unsigned char>();
    writeResponse(response, request, 0, serialized);
    socket->sendData(response);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    SString error("Error  : " + std::string(e.what()));
    auto serialized = error.serialize();
    writeResponse(response, request, 400, serialized);
    socket->sendData(response);
    return false;
  }
  return true;
}