#include "ApplicationFactory.h"
#include "IApplication.h"
#include "SignalHandler.h"
#include <iostream>
#include <memory>

void usage() {
  std::cerr << "Usage: ./FoodRecommendationSystem <server/client>\n";
  std::cerr << "server: Start server\n";
  std::cerr << "client: Start client\n";
}

int main(int argc, char const *argv[]) {
  ApplicationFactory applicationFactory;
  std::shared_ptr<IApplication> application =
      applicationFactory.createApplication(argc, argv);
  if (application == nullptr) {
    std::cerr << "Invalid arguments\n";
    usage();
    return 1;
  }
  SignalHandler signalHandler{application};
  signalHandler.setup();
  application->start();
  return 0;
}