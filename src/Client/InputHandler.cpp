#include "Client/InputHandler.h"

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