#pragma once

#include "IController.h"
#include "Server/Service/FoodItemService.h"
#include "Server/Service/MenuService.h"
#include "Server/Service/RecommendationService.h"
#include "Server/Service/UserService.h"
#include <functional>
#include <unordered_map>

namespace Controller {

class ChefController : public IController {
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
  void registerRoutes();

  bool getRecommendedMenu(std::shared_ptr<TcpSocket> socket,
                          TCPRequest &request,
                          std::vector<unsigned char> &payload);
  bool rolloutMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                   std::vector<unsigned char> &payload);
  bool createMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                  std::vector<unsigned char> &payload);
  bool viewRolloutMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                       std::vector<unsigned char> &payload);
  bool viewMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                std::vector<unsigned char> &payload);
  bool updateMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                  std::vector<unsigned char> &payload);
  bool viewRolloutFeedback(std::shared_ptr<TcpSocket> socket,
                           TCPRequest &request,
                           std::vector<unsigned char> &payload);
  bool viewNotifications(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload);
  bool viewFoodItems(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                     std::vector<unsigned char> &payload);

  bool addDiscardFeedbackQuestion(std::shared_ptr<TcpSocket> socket,
                                  TCPRequest &request,
                                  std::vector<unsigned char> &payload);
  bool discardFoodItem(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                       std::vector<unsigned char> &payload);
  bool viewDiscardFeedbackAnswerSentiments(std::shared_ptr<TcpSocket> socket,
                                           TCPRequest &request,
                                           std::vector<unsigned char> &payload);
  bool viewDiscardFeedbackQuestions(std::shared_ptr<TcpSocket> socket,
                                    TCPRequest &request,
                                    std::vector<unsigned char> &payload);
  bool getFoodItemsBelowRating(std::shared_ptr<TcpSocket> socket,
                               TCPRequest &request,
                               std::vector<unsigned char> &payload);
  bool getDiscardedFoodItems(std::shared_ptr<TcpSocket> socket,
                             TCPRequest &request,
                             std::vector<unsigned char> &payload);

public:
  ChefController(
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