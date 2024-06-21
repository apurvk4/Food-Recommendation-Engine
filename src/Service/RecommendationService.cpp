#include "Service/RecommendationService.h"
#include "FoodItemType.h"
#include "SentimentalAnalysis.h"

using Service::RecommendationService;

RecommendationService::RecommendationService(
    std::shared_ptr<DAO::IReviewDAO> reviewDAO,
    std::shared_ptr<DAO::FoodItemDAO> foodItemDAO)
    : reviewDAO(reviewDAO), foodItemDAO(foodItemDAO) {}

std::vector<DTO::FoodItem>
RecommendationService::getRecommendedFoodItems(DTO::FoodItemType foodItemType,
                                               U32 count) {
  std::string lexiconFilePath = "../lexicon.txt";
  SentimentAnalyzer sentimentAnalyzer(lexiconFilePath);

  std::vector<DTO::FoodItem> foodItems =
      foodItemDAO->getFoodItemsByType(foodItemType);
  std::vector<DTO::Review> reviews;
  for (auto &foodItem : foodItems) {
    std::vector<DTO::Review> foodItemReviews =
        reviewDAO->getReviewsByFoodItemId(foodItem.foodItemId);
    reviews.insert(reviews.end(), foodItemReviews.begin(),
                   foodItemReviews.end());
  }

  std::map<uint64_t, double> foodItemRatingSum;
  std::map<uint64_t, double> foodItemSentimentSum;
  std::map<uint64_t, uint64_t> foodItemReviewCount;
  for (auto &review : reviews) {
    double sentimentScore =
        sentimentAnalyzer.analyze(review.comment) == true ? 1.0 : -1.0;
    foodItemRatingSum[review.foodItemId] += review.rating;
    foodItemSentimentSum[review.foodItemId] += sentimentScore;
    foodItemReviewCount[review.foodItemId]++;
  }

  std::map<uint64_t, double> foodItemCombinedScore;
  for (const auto &pair : foodItemRatingSum) {
    uint64_t foodItemId = pair.first;
    double averageRating =
        foodItemRatingSum[foodItemId] / foodItemReviewCount[foodItemId];
    double averageSentiment =
        foodItemSentimentSum[foodItemId] / foodItemReviewCount[foodItemId];
    double combinedScore = (0.7 * averageRating) + (0.3 * averageSentiment);
    foodItemCombinedScore[foodItemId] = combinedScore;
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
