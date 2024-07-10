#include "Client/ClientApplication.h"
#include "Client/UserHandler.h"
#include "Controller/AdminController.h"
#include "Controller/AuthController.h"
#include "Controller/ChefController.h"
#include "Controller/EmployeeController.h"
#include "DAO/FeedbackDAO.h"
#include "DAO/MenuItemDAO.h"
#include "DAO/ReviewDAO.h"
#include "DAO/UserActivityDAO.h"
#include "DbConnection.h"
#include "DiscardFeedbackAnswerDAO.h"
#include "DiscardFeedbackQuestionDAO.h"
#include "FoodItemAttribute.h"
#include "FoodItemDAO.h"
#include "FoodItemService.h"
#include "IFeedbackDAO.h"
#include "IMenuItemDAO.h"
#include "IReviewDAO.h"
#include "IUserDAO.h"
#include "MenuDAO.h"
#include "MenuService.h"
#include "Middleware/AuthMiddleware.h"
#include "Middleware/IMiddleware.h"
#include "NotificationDAO.h"
#include "RecommendationService.h"
#include "Role.h"
#include "Server/FoodRecommendationServer.h"
#include "Server/RequestHandler.h"
#include "Server/RouteHandler.h"
#include "UserFoodPreferenceDAO.h"
#include "UserService.h"
#include <memory>

void initDbConnection() {
  DbConnection::initDbConnection("tcp://localhost:3306", "cppserver",
                                 "cppserver", "FoodRecommendationSystem");
}

void server() {
  initDbConnection();
  std::shared_ptr<DAO::IUserDAO> userDAO = std::make_shared<DAO::UserDAO>();

  std::shared_ptr<DAO::IUserActivityDAO> userActivityDAO =
      std::make_shared<DAO::UserActivityDAO>();

  std::shared_ptr<DAO::INotificationDAO> notificationDAO =
      std::make_shared<DAO::NotificationDAO>();

  std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO =
      std::make_shared<DAO::FoodItemDAO>();

  std::shared_ptr<DAO::IMenuDAO> menuDAO = std::make_shared<DAO::MenuDAO>();

  std::shared_ptr<DAO::IMenuItemDAO> menuItemDAO =
      std::make_shared<DAO::MenuItemDAO>();

  std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO =
      std::make_shared<DAO::FeedbackDAO>();

  std::shared_ptr<DAO::IReviewDAO> reviewDAO =
      std::make_shared<DAO::ReviewDAO>();

  std::shared_ptr<DAO::IFoodItemAttribute> foodItemAttributeDAO =
      std::make_shared<DAO::FoodItemAttribute>();

  std::shared_ptr<DAO::IUserFoodPreferenceDAO> userFoodPreferenceDAO =
      std::make_shared<DAO::UserFoodPreferenceDAO>();

  std::shared_ptr<DAO::IDiscardFeedbackAnswerDAO> discardFeedbackAnswerDAO =
      std::make_shared<DAO::DiscardFeedbackAnswerDAO>();

  std::shared_ptr<DAO::IDiscardFeedbackQuestionDAO> discardFeedbackQuestionDAO =
      std::make_shared<DAO::DiscardFeedbackQuestionDAO>();

  std::shared_ptr<Service::UserService> userService =
      std::make_shared<Service::UserService>(
          userDAO, userActivityDAO, notificationDAO, userFoodPreferenceDAO);

  std::shared_ptr<Service::FoodItemService> foodItemService =
      std::make_shared<Service::FoodItemService>(
          foodItemDAO, feedbackDAO, reviewDAO, foodItemAttributeDAO,
          discardFeedbackQuestionDAO, discardFeedbackAnswerDAO);

  std::shared_ptr<Service::MenuService> menuService =
      std::make_shared<Service::MenuService>(menuDAO, foodItemDAO, menuItemDAO);

  std::shared_ptr<Service::RecommendationService> recommendationService =
      std::make_shared<Service::RecommendationService>(reviewDAO, foodItemDAO);

  std::shared_ptr<Middleware::IMiddleware> employeeeAuthMiddleware =
      std::make_shared<Middleware::AuthMiddleware>(
          userService, (uint64_t)DTO::Role::Employee);

  std::shared_ptr<Middleware::IMiddleware> chefAuthMiddleware =
      std::make_shared<Middleware::AuthMiddleware>(userService,
                                                   (uint64_t)DTO::Role::Chef);

  std::shared_ptr<Middleware::IMiddleware> adminAuthMiddleware =
      std::make_shared<Middleware::AuthMiddleware>(userService,
                                                   (uint64_t)DTO::Role::Admin);

  std::shared_ptr<IController> authController =
      std::make_shared<Controller::AuthController>("/Auth", userService);

  std::shared_ptr<IController> employeeController =
      std::make_shared<Controller::EmployeeController>(
          "/Employee", userService, foodItemService, menuService,
          recommendationService);

  employeeController->registerPreprocessorMiddleware(employeeeAuthMiddleware);

  std::shared_ptr<IController> chefController =
      std::make_shared<Controller::ChefController>("/Chef", userService,
                                                   foodItemService, menuService,
                                                   recommendationService);

  chefController->registerPreprocessorMiddleware(chefAuthMiddleware);

  std::shared_ptr<IController> adminController =
      std::make_shared<Controller::AdminController>("/Admin", userService,
                                                    foodItemService);

  adminController->registerPreprocessorMiddleware(adminAuthMiddleware);

  RouteHandler routehandler;
  routehandler.registerController(authController);
  routehandler.registerController(employeeController);
  routehandler.registerController(chefController);
  routehandler.registerController(adminController);
  Server::RequestHandler handler{routehandler};

  std::cout << "Starting server s\n";
  Server::FoodRecommendationServer server{handler, 5};
  std::string ip{SERVER_IP};
  uint16_t serverPort = SERVER_PORT;

  std::cout << "Server started at " << ip << ":" << serverPort << "\n";
  server.start(Socket::convertIpAddress(ip), serverPort);
}

void client() {
  std::cout << "Starting client\n";
  ClientApplication app;
  app.run();
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <server/client>\n";
    return -1;
  }
  std::string mode = argv[1];
  if (mode == "s") {
    std::cout << "Starting server\n";
    server();
  } else if (mode == "c") {
    client();
  } else {
    std::cerr << "Invalid mode\n";
    return -1;
  }
  return 0;
}