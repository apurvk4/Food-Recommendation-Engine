#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
class Worker {
  std::atomic<bool> stopFlag;
  std::thread workerThread;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable taskCondition;
  void process();

public:
  Worker();
  ~Worker();
  void addTask(std::function<void()> task);
  void stop();
};