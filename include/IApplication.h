#pragma once

class IApplication {
public:
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual ~IApplication() = default;
};