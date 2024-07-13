#include "Service/RecommendationService.h"
#include "Category.h"
#include "FoodItem.h"
#include <algorithm>
#include <limits>

using Service::RecommendationService;

RecommendationService::RecommendationService(
    std::shared_ptr<DAO::IReviewDAO> reviewDAO,
    std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO)
    : reviewDAO(reviewDAO), foodItemDAO(foodItemDAO) {}

double RecommendationService::normalizeToRange(double value, double min,
                                               double max, double newMin,
                                               double newMax) {
  // Ensure value is within the range [min, max]
  double clampedValue = std::min(std::max(value, min), max);

  // Normalize to [0, 1]
  double normalizedValue = (clampedValue - min) / (max - min);

  // Map to [newMin, newMax] using logarithmic mapping
  double mappedValue =
      exp(normalizedValue * log(newMax - newMin + 1)) + newMin - 1;

  // Ensure the mapped value is within the new range [newMin, newMax]
  return std::min(std::max(mappedValue, newMin), newMax);
}

double RecommendationService::findSentimentScore(
    uint64_t foodItemId, SentimentAnalyzer &sentimentAnalyzer) {
  std::vector<DTO::Review> foodItemReviews =
      reviewDAO->getReviewsByFoodItemId(foodItemId);
  if (foodItemReviews.empty()) {
    return 3.0; // default average rating if no reviews present
  }

  double foodItemRatingSum = 0.0;
  double foodItemSentimentSum = 0.0;
  uint64_t foodItemReviewCount = 0;
  for (const auto &review : foodItemReviews) {
    double sentimentScore = sentimentAnalyzer.analyze(review.comment);
    foodItemRatingSum += review.rating;
    foodItemSentimentSum += sentimentScore;
    foodItemReviewCount++;
  }
  double averageRating = foodItemRatingSum / foodItemReviewCount;
  double averageSentiment = foodItemSentimentSum / foodItemReviewCount;
  double combinedScore =
      (0.7 * (averageRating / 5.0)) +
      (0.3 * (averageSentiment / 5.0)); // Now in range 0.0 to 1.0
  double normalizedScore = normalizeToRange(combinedScore, 0.0, 1.0, 1.0, 5.0);
  return normalizedScore;
}

double RecommendationService::getFoodItemRating(uint64_t foodItemId) {
  std::string lexiconFilePath = "../lexicon.txt";
  std::string stopWordsFilePath = "../stopWords.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath, stopWordsFilePath);

  double combinedScore = findSentimentScore(foodItemId, sentimentAnalyzer);
  return combinedScore;
}

std::vector<DTO::FoodItem>
RecommendationService::getRecommendedFoodItems(DTO::Category foodItemType,
                                               U32 count) {
  std::string lexiconFilePath = "../lexicon.txt";
  std::string stopWordsFilePath = "../stopWords.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath, stopWordsFilePath);

  std::vector<DTO::FoodItem> foodItems =
      foodItemDAO->getFoodItemsByType(foodItemType);

  std::map<uint64_t, double> foodItemCombinedScore;
  for (const auto &foodItem : foodItems) {
    double combinedScore =
        findSentimentScore(foodItem.foodItemId, sentimentAnalyzer);
    foodItemCombinedScore[foodItem.foodItemId] = combinedScore;
  }

  std::vector<std::pair<uint64_t, double>> foodItemScorePairs(
      foodItemCombinedScore.begin(), foodItemCombinedScore.end());
  std::sort(foodItemScorePairs.begin(), foodItemScorePairs.end(),
            [](std::pair<uint64_t, double> a, std::pair<uint64_t, double> b) {
              return a.second > b.second;
            });

  if (foodItemScorePairs.empty()) {
    throw std::invalid_argument(
        "No reviews found for the given food item type");
  }

  std::vector<DTO::FoodItem> recommendedFoodItems;
  for (const auto &pair : foodItemScorePairs) {
    uint64_t foodItemId = pair.first;
    auto it = std::find_if(foodItems.begin(), foodItems.end(),
                           [foodItemId](const DTO::FoodItem &item) {
                             return item.foodItemId == (U64)foodItemId;
                           });
    if (it != foodItems.end()) {
      recommendedFoodItems.push_back(*it);
    }
    if (recommendedFoodItems.size() >= count) {
      break;
    }
  }

  return recommendedFoodItems;
}

std::vector<DTO::FoodItem>
RecommendationService::getFoodItemsBelowRating(double rating) {
  std::vector<DTO::FoodItem> foodItems = foodItemDAO->getAllFoodItems();

  std::vector<std::pair<DTO::FoodItem, double>> foodItemRatingPairs;
  for (const auto &foodItem : foodItems) {
    double rating = getFoodItemRating(foodItem.foodItemId);
    foodItemRatingPairs.push_back({foodItem, rating});
  }
  std::sort(
      foodItemRatingPairs.begin(), foodItemRatingPairs.end(),
      [](std::pair<DTO::FoodItem, double> a,
         std::pair<DTO::FoodItem, double> b) { return a.second < b.second; });

  std::vector<DTO::FoodItem> foodItemsBelowRating;
  for (const auto &pair : foodItemRatingPairs) {
    if (pair.second < rating - std::numeric_limits<double>::epsilon()) {
      foodItemsBelowRating.push_back(pair.first);
    }
  }
  std::cout << "Found " << foodItemsBelowRating.size()
            << " food items below rating " << rating << std::endl;
  return foodItemsBelowRating;
}

std::vector<std::pair<double, std::vector<std::string>>>
RecommendationService::getSentiment(std::vector<std::string> responses) {
  std::string lexiconFilePath = "../lexicon.txt";
  std::string stopWordsFilePath = "../stopWords.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath, stopWordsFilePath);

  std::vector<std::pair<double, std::vector<std::string>>> sentimentScores;
  for (const auto &response : responses) {
    double sentimentScore = sentimentAnalyzer.analyze(response);
    auto frequentWords = sentimentAnalyzer.frequentWords(responses, 5);
    std::vector<std::string> words;
    for (const auto &pair : frequentWords) {
      words.push_back(pair.first);
    }
    sentimentScores.push_back({sentimentScore, words});
  }
  return sentimentScores;
}