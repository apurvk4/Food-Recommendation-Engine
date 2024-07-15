#include "SignalHandler.h"
#include "IApplication.h"
#include <csignal>
#include <functional>
#include <iostream>
#include <unistd.h>

std::shared_ptr<IApplication> SignalHandler::application = nullptr;

SignalHandler::SignalHandler(std::shared_ptr<IApplication> application) {
  SignalHandler::application = application;
}

void SignalHandler::setup() {
  signal(SIGTERM, handleSignal);
  signal(SIGINT, handleSignal);
}

void SignalHandler::stopApplication() {
  std::cout << "stopping application\n";
  application->stop();
}

void SignalHandler::handleSignal(int signal) {
  if (signal == SIGTERM or signal == SIGINT) {
    stopApplication();
  }
}