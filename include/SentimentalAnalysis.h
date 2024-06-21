#pragma once

#include "BoWModel.h"
#include "SentimentLexicon.h"
#include "Tokenizer.h"
#include <string>

class SentimentAnalyzer {
private:
  Tokenizer tokenizer;
  SentimentLexicon lexicon;

public:
  SentimentAnalyzer(const std::string &lexiconPath = "lexicon.txt")
      : lexicon(lexiconPath) {}

  bool analyze(const std::string &comment) {
    auto tokens = tokenizer.tokenize(comment);
    BoWModel model;
    model.buildModel(tokens);

    int sentimentScore = 0;
    for (const auto &pair : model.getWordCount()) {
      sentimentScore += pair.second * lexicon.getSentiment(pair.first);
    }

    return sentimentScore >= 0;
  }
};
