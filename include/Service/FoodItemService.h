#pragma once

#include "Category.h"
#include "DAO/IFeedbackDAO.h"
#include "DAO/IFoodItemDAO.h"
#include "DTO/Feedback.h"
#include "DTO/FoodItem.h"
#include "DTO/Review.h"
#include "IFoodItemAttribute.h"
#include "IReviewDAO.h"
#include <memory>

namespace Service {

using DAO::IFeedbackDAO;
using DAO::IFoodItemDAO;
using DAO::IReviewDAO;
using DTO::Feedback;
using DTO::FoodItem;
using DTO::Review;

class FoodItemService {
  std::shared_ptr<DAO::IFoodItemDAO> m_foodItemDAO;
  std::shared_ptr<DAO::IFeedbackDAO> m_feedbackDAO;
  std::shared_ptr<DAO::IReviewDAO> m_reviewDAO;
  std::shared_ptr<DAO::IFoodItemAttribute> m_foodItemAttributeDAO;

public:
  FoodItemService(
      std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
      std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO,
      std::shared_ptr<DAO::IReviewDAO> reviewDAO,
      std::shared_ptr<DAO::IFoodItemAttribute> foodItemAttributeDAO);
  bool addFoodItem(FoodItem foodItem);
  bool updateFoodItem(FoodItem foodItem);
  bool deleteFoodItem(uint64_t foodItemId);
  FoodItem getFoodItemById(uint64_t foodItemId);
  std::vector<FoodItem> getAllFoodItems();
  std::vector<FoodItem> getFoodItemsByCategory(DTO::Category category);
  bool addFeedback(Feedback feedback);
  std::vector<Feedback> getAllFeedback(uint64_t foodItemId, std::string date);
  std::vector<Feedback> getFeedbackByUserId(uint64_t userId);
  std::vector<Feedback> getFeedbackByUserId(uint64_t userId, std::string date);
  bool addReview(DTO::Review review);
  std::vector<Review> getAllReviews(uint64_t foodItemId);
  std::vector<Review> getAllReviews(uint64_t foodItemId, std::string date);
  std::vector<Review> getReviewsByUserId(uint64_t userId);
  std::vector<Review> getReviewsByUserId(uint64_t userId, uint64_t foodItemId);
  std::vector<Review> getReviewsByUserId(uint64_t userId, uint64_t foodItemId,
                                         std::string date);
  bool addAttributeToFoodItem(uint64_t foodItemId, uint64_t attributeId);
  std::vector<std::pair<uint64_t, std::string>>
  getFoodItemAttributes(uint64_t foodItemId);
  std::vector<std::pair<uint64_t, std::string>> getAllAttributes();
  bool removeAttributeFromFoodItem(uint64_t foodItemId, uint64_t attributeId);
  bool addAttributesToFoodItem(std::vector<uint64_t> attributeIds,
                               uint64_t foodItemId);
  std::vector<DTO::FoodItem>
  getFoodItemsWithGivenAttributes(uint64_t attributeId);
  std::vector<std::pair<uint64_t,std::string>> getAttributeDetails(const std::vector<uint64_t>& attributeId);
};

}; // namespace Service