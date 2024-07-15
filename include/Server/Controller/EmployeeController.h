#pragma once

#include "IController.h"
#include "Server/Service/FoodItemService.h"
#include "Server/Service/MenuService.h"
#include "Server/Service/RecommendationService.h"
#include "Server/Service/UserService.h"

namespace Controller {

class EmployeeController : public IController {
  std::vector<std::shared_ptr<IMiddleware>> preprocessors;
  std::vector<std::shared_ptr<IMiddleware>> postprocessors;

  std::shared_ptr<Service::UserService> userService;
  std::shared_ptr<Service::FoodItemService> foodItemService;
  std::shared_ptr<Service::MenuService> menuService;
  std::shared_ptr<Service::RecommendationService> recommendationService;

  std::string baseAuthEndpoint;
  std::unordered_map<
      std::string,
      std::function<bool(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload)>>
      authRoutes;

  bool viewMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                std::vector<unsigned char> &payload);
  bool viewMenuRollout(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                       std::vector<unsigned char> &payload);
  bool sendFeedback(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                    std::vector<unsigned char> &payload);
  bool viewNotifications(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload);
  bool writeReview(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                   std::vector<unsigned char> &payload);
  bool getAttributes(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                     std::vector<unsigned char> &payload);
  bool viewFoodItemAttributes(std::shared_ptr<TcpSocket> socket,
                              TCPRequest &request,
                              std::vector<unsigned char> &payload);
  bool addFoodPreferences(std::shared_ptr<TcpSocket> socket,
                          TCPRequest &request,
                          std::vector<unsigned char> &payload);
  bool deleteFoodPreferences(std::shared_ptr<TcpSocket> socket,
                             TCPRequest &request,
                             std::vector<unsigned char> &payload);
  bool getFoodPreferences(std::shared_ptr<TcpSocket> socket,
                          TCPRequest &request,
                          std::vector<unsigned char> &payload);
  bool getDiscardedFoodItems(std::shared_ptr<TcpSocket> socket,
                             TCPRequest &request,
                             std::vector<unsigned char> &payload);
  bool getDiscardedFoodItemQuestions(std::shared_ptr<TcpSocket> socket,
                                     TCPRequest &request,
                                     std::vector<unsigned char> &payload);
  bool addDiscardedFoodItemAnswer(std::shared_ptr<TcpSocket> socket,
                                  TCPRequest &request,
                                  std::vector<unsigned char> &payload);
  void sortFoodItems(uint64_t userId,
                     std::vector<Service::FoodItem> &foodItems);

public:
  EmployeeController(
      const std::string &authEndpoint,
      std::shared_ptr<Service::UserService> userService,
      std::shared_ptr<Service::FoodItemService> foodItemService,
      std::shared_ptr<Service::MenuService> menuService,
      std::shared_ptr<Service::RecommendationService> recommendationService);
  bool handleRequest(TcpSocket socket, TCPRequest &request,
                     std::vector<unsigned char> &payload) override;
  std::string getEndpoint() override;
  void registerPreprocessorMiddleware(
      std::shared_ptr<Middleware::IMiddleware> middleware) override;
  void registerPostprocessorMiddleware(
      std::shared_ptr<Middleware::IMiddleware> middleware) override;
};

}; // namespace Controller