#include "EmployeeService.h"
#include "Feedback.h"
#include "IFeedbackDAO.h"
#include "IMenuDAO.h"
#include "User.h"
#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <sstream>

std::string getCurrentDateWithMidnightTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);
  now_tm.tm_hour = 0;
  now_tm.tm_min = 0;
  now_tm.tm_sec = 0;
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

bool EmployeeService::verifyRole(uint64_t userId) {
  User user = getUserById(userId);
  return user.roleId == Roles::Employee;
}

EmployeeService::EmployeeService(std::shared_ptr<IUserDAO> userDAO,
                                 std::shared_ptr<IMenuDAO> menuDAO,
                                 std::shared_ptr<IFeedbackDAO> feedbackDAO,
                                 User activeUser)
    : UserService(userDAO, activeUser), menuDAO(menuDAO),
      feedbackDAO(feedbackDAO) {}

std::vector<Menu> EmployeeService::getTodaysMenu() {
  Roles role = getCurrentRole();
  if (role > Roles::Employee) {
    throw std::runtime_error("User does not have permission to view menu");
  }
  return menuDAO->getMenusByDate(getCurrentDateWithMidnightTime());
}

bool EmployeeService::sendFeedback(uint64_t foodItemId, uint64_t userId,
                                   std::string comment, int rating) {
  try {
    Roles role = getCurrentRole();
    if (role > Roles::Employee) {
      throw std::runtime_error(
          "User does not have permission to send feedback");
    }
    Feedback feedback{0, userId, foodItemId, comment, rating, ""};
    return feedbackDAO->addFeedback(feedback);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}