#pragma once

#include "Controller/IController.h"
#include "FoodItemService.h"
#include "MenuService.h"
#include "RecommendationService.h"
#include "UserService.h"
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

  bool getRecommendedMenu(std::shared_ptr<TcpSocket> socket,
                          TCPRequest &request,
                          std::vector<unsigned char> &payload);
  bool rolloutMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                   std::vector<unsigned char> &payload);
  bool createMenu(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                  std::vector<unsigned char> &payload);
  bool viewRolloutFeedback(std::shared_ptr<TcpSocket> socket,
                           TCPRequest &request,
                           std::vector<unsigned char> &payload);
  bool viewNotifications(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload);
  bool viewFoodItems(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
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