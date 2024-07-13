#pragma once

#include "IApplication.h"
#include <csignal>
#include <memory>

class SignalHandler {
  static std::shared_ptr<IApplication> application;
  static void stopApplication();

public:
  SignalHandler(std::shared_ptr<IApplication> application);
  void setup();
  static void handleSignal(int signal);
};