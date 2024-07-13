#pragma once

#include "Server/Worker.h"
#include <cstddef>
#include <memory>
#include <vector>
class LoadBalancer {
  std::vector<std::shared_ptr<Worker>> workers;

public:
  LoadBalancer(size_t numWorkers);
  void distributeTask(std::function<void()> task);
  void stop();
};