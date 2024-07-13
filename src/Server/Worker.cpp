#include "Server/Worker.h"
#include <iostream>

Worker::Worker() : stopFlag(false), workerThread(&Worker::process, this) {}

Worker::~Worker() { stop(); }

void Worker::addTask(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    tasks.push(task);
  }
  taskCondition.notify_one();
}

void Worker::stop() {
  stopFlag = true;
  taskCondition.notify_one();
  if (workerThread.joinable()) {
    workerThread.join();
  }
}

void Worker::process() {
  while (!stopFlag) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      taskCondition.wait_for(lock, std::chrono::seconds(1),
                             [this] { return !tasks.empty() || stopFlag; });
      if (stopFlag) {
        return;
      }
      if (tasks.empty()) {
        continue;
      }
      task = tasks.front();
      tasks.pop();
    }
    try {
      task();
    } catch (const std::exception &e) {
      std::cerr << "Exception in worker thread: " << e.what() << std::endl;
    } catch (...) {
      std::cerr << "Unknown exception in worker thread" << std::endl;
    }
  }
}