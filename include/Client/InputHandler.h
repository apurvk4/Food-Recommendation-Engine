#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
class InputHandler {
  inline void exitIfInputClosed();
  inline bool isInputFailed();
  inline void clearErrorState();
  inline void clearBuffer();
  inline void resetCin();
  inline void showErrorMessage();
  inline void handleInvalidInput();

public:
  template <typename T>
  T getInput(const std::function<bool(const T &)> &rule = nullptr);
};
void InputHandler::exitIfInputClosed() {
  if (std::cin.eof()) {
    std::cout << "\nError : Input closed Unexpectedly. Exiting ... \n";
    exit(EXIT_FAILURE);
  }
}
bool InputHandler::isInputFailed() {
  char nextChar = std::cin.peek();
  return (std::cin.fail() || (nextChar != EOF && nextChar != '\n'));
}
void InputHandler::clearErrorState() { std::cin.clear(); }
void InputHandler::clearBuffer() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void InputHandler::resetCin() {
  clearErrorState();
  clearBuffer();
}
void InputHandler::showErrorMessage() {
  std::cout << "\nInvalid Input. Please Retry..." << "\n";
}
void InputHandler::handleInvalidInput() {
  resetCin();
  showErrorMessage();
}
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