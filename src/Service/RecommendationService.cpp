#include "Service/RecommendationService.h"
#include "Category.h"
#include "FoodItem.h"
#include "SentimentalAnalysis.h"
#include <algorithm>
#include <limits>

using Service::RecommendationService;

RecommendationService::RecommendationService(
    std::shared_ptr<DAO::IReviewDAO> reviewDAO,
    std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO)
    : reviewDAO(reviewDAO), foodItemDAO(foodItemDAO) {}

double RecommendationService::normalizeScore(double score, double min,
                                             double max) {
  return 1.0 + 4.0 * (score - min) / (max - min);
}

double RecommendationService::findSentimentScore(
    uint64_t foodItemId, SentimentAnalyzer &sentimentAnalyzer,
    double &minSentiment, double &maxSentiment) {
  std::vector<DTO::Review> foodItemReviews =
      reviewDAO->getReviewsByFoodItemId(foodItemId);
  if (foodItemReviews.empty()) {
    return 3.0; // default average rating if no reviews present
  }

  double sentimentSum = 0;
  double ratingSum = 0;
  for (auto &review : foodItemReviews) {
    double sentimentScore = sentimentAnalyzer.analyze(review.comment);
    sentimentSum += sentimentScore;
    ratingSum += review.rating;
    minSentiment = std::min(minSentiment, sentimentScore);
    maxSentiment = std::max(maxSentiment, sentimentScore);
  }

  double averageSentiment = sentimentSum / foodItemReviews.size();
  double averageRating = ratingSum / foodItemReviews.size();

  return (0.7 * averageRating) + (0.3 * averageSentiment);
}

double RecommendationService::getFoodItemRating(uint64_t foodItemId) {
  std::string lexiconFilePath = "../lexicon.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath);

  double minSentiment = -1;
  double maxSentiment = -1;

  double combinedScore = findSentimentScore(foodItemId, sentimentAnalyzer,
                                            minSentiment, maxSentiment);

  if (minSentiment != -1 && maxSentiment != -1) {
    double result = normalizeScore(combinedScore, minSentiment, maxSentiment);
  }
  return combinedScore;
}

std::vector<DTO::FoodItem>
RecommendationService::getRecommendedFoodItems(DTO::Category foodItemType,
                                               U32 count) {
  std::string lexiconFilePath = "../lexicon.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath);

  std::vector<DTO::FoodItem> foodItems =
      foodItemDAO->getFoodItemsByType(foodItemType);

  double minSentiment = std::numeric_limits<double>::max();
  double maxSentiment = std::numeric_limits<double>::min();

  std::map<uint64_t, double> foodItemCombinedScore;
  for (const auto &foodItem : foodItems) {
    double combinedScore = findSentimentScore(
        foodItem.foodItemId, sentimentAnalyzer, minSentiment, maxSentiment);
    foodItemCombinedScore[foodItem.foodItemId] = combinedScore;
  }

  // Normalize the combined scores to be between 1 and 5
  for (auto &pair : foodItemCombinedScore) {
    pair.second = normalizeScore(pair.second, minSentiment, maxSentiment);
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
  return foodItemsBelowRating;
}