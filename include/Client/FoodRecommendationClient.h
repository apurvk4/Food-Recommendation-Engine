#pragma once

#include <cstdint>
#include <string>
class FoodRecommendationClient {
public:
  void loginUser(uint64_t userId, std::string password, uint64_t roleId);
};