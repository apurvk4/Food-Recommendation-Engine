#pragma once

#include "FoodItem.h"
#include "SerializableTypes/ProtocolDefinitions.h"
#include "User.h"
#include <cstdint>
#include <string>
#include <vector>
class FoodRecommendationClient {
  DTO::User *activeUser;
  void writeProtocolHeader(std::vector<unsigned char> &buffer,
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
  std::vector<DTO::FoodItem> getFoodItems();
  std::vector<DTO::FoodItem> getRecommendedFoodItems();

public:
  FoodRecommendationClient() : activeUser(nullptr){};
  void loginUser();
  void logout();
  void showFoodItems();
  void addFoodItem();
  void updateFoodItem();
  void deleteFoodItem();
  int getCurrentRole();
  void showFoodItemRecommendation();
};