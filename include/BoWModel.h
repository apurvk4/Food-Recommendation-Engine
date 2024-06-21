#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class BoWModel {
private:
  std::unordered_map<std::string, int> wordCount;

public:
  void buildModel(const std::vector<std::string> &tokens) {
    for (const auto &token : tokens) {
      wordCount[token]++;
    }
  }

  const std::unordered_map<std::string, int> &getWordCount() const {
    return wordCount;
  }
};
