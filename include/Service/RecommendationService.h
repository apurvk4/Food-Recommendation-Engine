#pragma once

#include "Category.h"
#include "FoodItem.h"
#include "FoodItemDAO.h"
#include "IReviewDAO.h"
#include "Menu.h"
#include "SentimentalAnalysis.h"
#include <memory>

namespace Service {

class RecommendationService {
private:
  std::shared_ptr<DAO::IReviewDAO> reviewDAO;
  std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO;

  double normalizeScore(double score, double min, double max);
  double findSentimentScore(uint64_t foodItemId,
                            SentimentAnalyzer &sentimentAnalyzer,
                            double &minSentiment, double &maxSentiment);

public:
  RecommendationService(std::shared_ptr<DAO::IReviewDAO> ReviewDAO,
                        std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO);
  std::vector<DTO::FoodItem> getRecommendedFoodItems(DTO::Category, U32 count);
  double getFoodItemRating(uint64_t foodItemId);
};

}; // namespace Service