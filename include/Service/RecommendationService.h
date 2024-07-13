#pragma once

#include "Category.h"
#include "FoodItem.h"
#include "FoodItemDAO.h"
#include "IReviewDAO.h"
#include "Menu.h"
#include "SentimentalAnalyzer.h"
#include <memory>

namespace Service {

class RecommendationService {
private:
  std::shared_ptr<DAO::IReviewDAO> reviewDAO;
  std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO;

  double normalizeToRange(double value, double min, double max, double newMin,
                          double newMax);
  double findSentimentScore(uint64_t foodItemId,
                            SentimentAnalyzer &sentimentAnalyzer);

public:
  RecommendationService(std::shared_ptr<DAO::IReviewDAO> ReviewDAO,
                        std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO);
  std::vector<DTO::FoodItem> getRecommendedFoodItems(DTO::Category, U32 count);
  double getFoodItemRating(uint64_t foodItemId);
  std::vector<DTO::FoodItem> getFoodItemsBelowRating(double rating);
  std::vector<std::pair<double, std::vector<std::string>>>
  getSentiment(std::vector<std::string> responses);
};

}; // namespace Service