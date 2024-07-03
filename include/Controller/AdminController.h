#pragma once

#include "Controller/IController.h"
#include "FoodItemService.h"
#include "UserService.h"
namespace Controller {

class AdminController : public IController {
  std::vector<std::shared_ptr<IMiddleware>> preprocessors;
  std::vector<std::shared_ptr<IMiddleware>> postprocessors;

  std::shared_ptr<Service::UserService> userService;
  std::shared_ptr<Service::FoodItemService> foodItemService;

  std::string baseAuthEndpoint;
  std::unordered_map<
      std::string,
      std::function<bool(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload)>>
      authRoutes;

  bool addFoodItem(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                   std::vector<unsigned char> &payload);
  bool removeFoodItem(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                      std::vector<unsigned char> &payload);
  bool updateFoodItem(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                      std::vector<unsigned char> &payload);
  bool getFoodItems(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                    std::vector<unsigned char> &payload);
  bool addUser(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
               std::vector<unsigned char> &payload);
  bool viewNotifications(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                         std::vector<unsigned char> &payload);
  bool addAttribute(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                    std::vector<unsigned char> &payload);
  bool getAllAttributes(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                        std::vector<unsigned char> &payload);
  bool viewFoodItemAttributes(std::shared_ptr<TcpSocket> socket,
                              TCPRequest &request,
                              std::vector<unsigned char> &payload);
  bool removeAttribute(std::shared_ptr<TcpSocket> socket, TCPRequest &request,
                       std::vector<unsigned char> &payload);

public:
  AdminController(const std::string &authEndpoint,
                  std::shared_ptr<Service::UserService> userService,
                  std::shared_ptr<Service::FoodItemService> foodItemService);
  bool handleRequest(TcpSocket socket, TCPRequest &request,
                     std::vector<unsigned char> &payload) override;
  std::string getEndpoint() override;
  void registerPreprocessorMiddleware(
      std::shared_ptr<Middleware::IMiddleware> middleware) override;
  void registerPostprocessorMiddleware(
      std::shared_ptr<Middleware::IMiddleware> middleware) override;
};

}; // namespace Controller