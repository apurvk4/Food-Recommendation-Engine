#include "Server/LoadBalancer.h"

LoadBalancer::LoadBalancer(size_t numWorkers) {
  for (size_t i = 0; i < numWorkers; ++i) {
    workers.push_back(std::make_shared<Worker>());
  }
}

void LoadBalancer::distributeTask(std::function<void()> task) {
  static size_t nextWorker = 0;
  workers[nextWorker]->addTask(task);
  nextWorker = (nextWorker + 1) % workers.size();
}

void LoadBalancer::stop() {
  for (auto &worker : workers) {
    worker->stop();
  }
}
