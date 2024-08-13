#include "ApplicationFactory.h"

#include "Client/ClientApplication.h"
#include "Client/UserHandler.h"
#include "Config.h"
#include "IApplication.h"
#include "Role.h"
#include "Server/Controller/AdminController.h"
#include "Server/Controller/AuthController.h"
#include "Server/Controller/ChefController.h"
#include "Server/Controller/EmployeeController.h"
#include "Server/DAO/DiscardFeedbackAnswerDAO.h"
#include "Server/DAO/DiscardFeedbackQuestionDAO.h"
#include "Server/DAO/FeedbackDAO.h"
#include "Server/DAO/FoodItemAttributeDAO.h"
#include "Server/DAO/FoodItemDAO.h"
#include "Server/DAO/IFeedbackDAO.h"
#include "Server/DAO/IMenuItemDAO.h"
#include "Server/DAO/IReviewDAO.h"
#include "Server/DAO/IUserDAO.h"
#include "Server/DAO/MenuDAO.h"
#include "Server/DAO/MenuItemDAO.h"
#include "Server/DAO/NotificationDAO.h"
#include "Server/DAO/ReviewDAO.h"
#include "Server/DAO/UserActivityDAO.h"
#include "Server/DAO/UserFoodPreferenceDAO.h"
#include "Server/DbConnection.h"
#include "Server/Middleware/AuthMiddleware.h"
#include "Server/Middleware/IMiddleware.h"
#include "Server/RequestHandler.h"
#include "Server/RouteHandler.h"
#include "Server/ServerApplication.h"
#include "Server/Service/FoodItemService.h"
#include "Server/Service/MenuService.h"
#include "Server/Service/RecommendationService.h"
#include "Server/Service/UserService.h"
#include <csignal>
#include <memory>

void initDbConnection() {
  std::string userName = std::getenv("MYSQL_USER");
  std::string password = std::getenv("MYSQL_PASSWORD");
  std::string host = std::getenv("MYSQL_HOST");
  std::string port = std::getenv("MYSQL_PORT");
  std::string dbName = std::getenv("MYSQL_DB");

  if (userName.empty() || password.empty() || host.empty() || port.empty() ||
      dbName.empty()) {
    std::cerr << "Environment variables not set\n";
    std::cerr << "MYSQL_USER, MYSQL_PASSWORD, MYSQL_HOST, MYSQL_PORT, MYSQL_DB "
                 "should be set\n";
    std::cerr << "If Not done already, create the Database and Tables using "
                 "the sql file\n";
    std::cerr << "And then set the environment variables\n";
    exit(1);
  }

  DbConnection::initDbConnection("tcp://" + host + ":" + port, userName,
                                 password, dbName);
}

IApplication *ApplicationFactory::createClientApplication() {
  return new ClientApplication();
}

IApplication *ApplicationFactory::createServerApplication() {
  initDbConnection();
  // DAOs
  auto userDAO = std::make_shared<DAO::UserDAO>();
  auto userActivityDAO = std::make_shared<DAO::UserActivityDAO>();
  auto notificationDAO = std::make_shared<DAO::NotificationDAO>();
  auto foodItemDAO = std::make_shared<DAO::FoodItemDAO>();
  auto menuDAO = std::make_shared<DAO::MenuDAO>();
  auto menuItemDAO = std::make_shared<DAO::MenuItemDAO>();
  auto feedbackDAO = std::make_shared<DAO::FeedbackDAO>();
  auto reviewDAO = std::make_shared<DAO::ReviewDAO>();
  auto foodItemAttributeDAO = std::make_shared<DAO::FoodItemAttributeDAO>();
  auto userFoodPreferenceDAO = std::make_shared<DAO::UserFoodPreferenceDAO>();
  auto discardFeedbackAnswerDAO =
      std::make_shared<DAO::DiscardFeedbackAnswerDAO>();
  auto discardFeedbackQuestionDAO =
      std::make_shared<DAO::DiscardFeedbackQuestionDAO>();
  auto attributeDAO = std::make_shared<DAO::AttributeDAO>();

  // Services
  auto userService = std::make_shared<Service::UserService>(
      userDAO, userActivityDAO, notificationDAO, userFoodPreferenceDAO);
  auto foodItemService = std::make_shared<Service::FoodItemService>(
      foodItemDAO, feedbackDAO, reviewDAO, foodItemAttributeDAO,
      discardFeedbackQuestionDAO, discardFeedbackAnswerDAO, attributeDAO);
  auto menuService =
      std::make_shared<Service::MenuService>(menuDAO, foodItemDAO, menuItemDAO);
  auto recommendationService =
      std::make_shared<Service::RecommendationService>(reviewDAO, foodItemDAO);

  // Middleware
  auto employeeAuthMiddleware = std::make_shared<Middleware::AuthMiddleware>(
      userService, static_cast<uint64_t>(DTO::Role::Employee));
  auto chefAuthMiddleware = std::make_shared<Middleware::AuthMiddleware>(
      userService, static_cast<uint64_t>(DTO::Role::Chef));
  auto adminAuthMiddleware = std::make_shared<Middleware::AuthMiddleware>(
      userService, static_cast<uint64_t>(DTO::Role::Admin));

  // Controllers
  auto authController =
      std::make_shared<Controller::AuthController>("/Auth", userService);
  auto employeeController = std::make_shared<Controller::EmployeeController>(
      "/Employee", userService, foodItemService, menuService,
      recommendationService);
  employeeController->registerPreprocessorMiddleware(employeeAuthMiddleware);

  auto chefController = std::make_shared<Controller::ChefController>(
      "/Chef", userService, foodItemService, menuService,
      recommendationService);
  chefController->registerPreprocessorMiddleware(chefAuthMiddleware);

  auto adminController = std::make_shared<Controller::AdminController>(
      "/Admin", userService, foodItemService);
  adminController->registerPreprocessorMiddleware(adminAuthMiddleware);

  // Route Handler
  std::unique_ptr<RouteHandler> routehandler = std::make_unique<RouteHandler>();
  routehandler->registerController(authController);
  routehandler->registerController(employeeController);
  routehandler->registerController(chefController);
  routehandler->registerController(adminController);

  // Server
  std::unique_ptr<Server::RequestHandler> handler =
      std::make_unique<Server::RequestHandler>(std::move(routehandler));
  std::string ip{SERVER_IP};
  uint16_t serverPort = SERVER_PORT;

  std::cout << "Starting server at " << ip << ":" << serverPort << "\n";
  IApplication *application = new Server::ServerApplication{
      std::move(handler), Socket::convertIpAddress(ip), serverPort};
  return application;
}

std::shared_ptr<IApplication>
ApplicationFactory::createApplication(int argc, const char **argv) {
  if (argc != 2) {
    return nullptr;
  }
  std::string mode = argv[1];
  if (mode == "server") {
    return std::shared_ptr<IApplication>(createServerApplication());
  } else if (mode == "client") {
    return std::shared_ptr<IApplication>(createClientApplication());
  }
  return nullptr;
}
