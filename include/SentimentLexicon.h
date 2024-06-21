#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class SentimentLexicon {
private:
  std::unordered_map<std::string, int> lexicon;

public:
  SentimentLexicon(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << std::endl;
      return;
    }
    std::string word;
    int sentiment;
    while (file >> word >> sentiment) {
      lexicon[word] = sentiment;
    }
  }

  int getSentiment(const std::string &word) const {
    auto it = lexicon.find(word);
    if (it != lexicon.end()) {
      return it->second;
    }
    return 0;
  }
};