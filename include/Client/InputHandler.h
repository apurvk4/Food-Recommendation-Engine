#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
class InputHandler {
  void exitIfInputClosed();
  bool isInputFailed();
  void clearErrorState();
  void clearBuffer();
  void resetCin();
  void showErrorMessage();
  void handleInvalidInput();

public:
  template <typename T>
  T getInput(const std::function<bool(const T &)> &rule = nullptr);
};

template <typename T>
T InputHandler::getInput(const std::function<bool(const T &)> &rule) {
  T value;
  while (true) {
    std::cin >> value;
    exitIfInputClosed();
    if (isInputFailed()) {
      handleInvalidInput();
    } else {
      bool isUserRuleSatisfied = rule == nullptr ? true : rule(value);
      if (isUserRuleSatisfied) {
        break;
      } else {
        handleInvalidInput();
      }
    }
  }
  return value;
}