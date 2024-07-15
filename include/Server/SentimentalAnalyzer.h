#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SentimentAnalyzer {
  std::unordered_map<std::string, int> wordScores;
  std::unordered_set<std::string> stopWords;

  void loadLexicon(const std::string &lexiconFilePath);
  void loadStopWords(const std::string &stopWordsFilePath);
  std::vector<std::string> tokenize(const std::string &text) const;
  std::vector<std::string>
  removeStopWords(const std::vector<std::string> &tokens) const;

public:
  SentimentAnalyzer(const std::string &lexiconFilePath,
                    const std::string &stopWordsFilePath);
  bool isStopWord(const std::string &word) const;
  double analyze(const std::string &text) const;
  std::unordered_map<std::string, int>
  frequentWords(const std::vector<std::string> &texts, int topN) const;
};
