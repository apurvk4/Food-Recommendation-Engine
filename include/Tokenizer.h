#pragma once

#include <sstream>
#include <string>
#include <vector>

class Tokenizer {
public:
  std::vector<std::string> tokenize(const std::string &text) const {
    std::vector<std::string> tokens;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
      tokens.push_back(word);
    }
    return tokens;
  }
};
