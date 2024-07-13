#include "SentimentalAnalyzer.h"

void SentimentAnalyzer::loadLexicon(const std::string &lexiconFilePath) {
  std::ifstream file(lexiconFilePath);
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string word;
    int score;
    if (iss >> word >> score) {
      wordScores[word] = score;
    }
  }
}

void SentimentAnalyzer::loadStopWords(const std::string &stopWordsFilePath) {
  std::ifstream file(stopWordsFilePath);
  std::string word;
  while (std::getline(file, word, ',')) {
    stopWords.insert(word);
  }
}

std::vector<std::string>
SentimentAnalyzer::tokenize(const std::string &text) const {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::string word;

  while (iss >> word) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    tokens.push_back(word);
  }

  return tokens;
}

std::vector<std::string> SentimentAnalyzer::removeStopWords(
    const std::vector<std::string> &tokens) const {
  std::vector<std::string> filteredTokens;
  for (const auto &token : tokens) {
    if (!isStopWord(token)) {
      filteredTokens.push_back(token);
    }
  }
  return filteredTokens;
}

SentimentAnalyzer::SentimentAnalyzer(const std::string &lexiconFilePath,
                                     const std::string &stopWordsFilePath) {
  loadLexicon(lexiconFilePath);
  loadStopWords(stopWordsFilePath);
}

bool SentimentAnalyzer::isStopWord(const std::string &word) const {
  return stopWords.find(word) != stopWords.end();
}

double SentimentAnalyzer::analyze(const std::string &text) const {
  std::vector<std::string> tokens = tokenize(text);
  tokens = removeStopWords(tokens);

  double score = 0.0;
  int wordCount = 0;

  for (const auto &token : tokens) {
    auto it = wordScores.find(token);
    if (it != wordScores.end()) {
      score += it->second;
      ++wordCount;
    }
  }
  if (wordCount > 0) {
    score /= wordCount;
  }

  double normalizedScore = 1.0 + (score + 5.0) / 2.0;

  if (normalizedScore < 1.0) {
    normalizedScore = 1.0;
  } else if (normalizedScore > 5.0) {
    normalizedScore = 5.0;
  }

  return normalizedScore;
}

std::unordered_map<std::string, int>
SentimentAnalyzer::frequentWords(const std::vector<std::string> &texts,
                                 int topN) const {
  std::unordered_map<std::string, int> wordCounts;

  for (const auto &text : texts) {
    std::vector<std::string> tokens = tokenize(text);
    tokens = removeStopWords(tokens);
    for (const auto &token : tokens) {
      wordCounts[token]++;
    }
  }
  std::vector<std::pair<std::string, int>> sortedWords(wordCounts.begin(),
                                                       wordCounts.end());
  std::sort(sortedWords.begin(), sortedWords.end(),
            [](const auto &a, const auto &b) {
              return a.second > b.second; // Sort by descending count
            });

  std::unordered_map<std::string, int> topWordsMap;
  for (int i = 0; i < std::min(topN, static_cast<int>(sortedWords.size()));
       ++i) {
    topWordsMap[sortedWords[i].first] = sortedWords[i].second;
  }

  return topWordsMap;
}
