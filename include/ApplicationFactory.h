#pragma once

#include "IApplication.h"
#include <memory>

class ApplicationFactory {
  IApplication *createClientApplication();
  IApplication *createServerApplication();

public:
  ApplicationFactory() = default;
  std::shared_ptr<IApplication> createApplication(int argc, const char **argv);
};