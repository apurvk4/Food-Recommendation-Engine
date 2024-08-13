#pragma once

#include "Category.h"
#include "DTO/Feedback.h"
#include "DTO/FoodItem.h"
#include "DTO/Review.h"
#include "Server/DAO/AttributeDAO.h"
#include "Server/DAO/IDiscardFeedbackAnswerDAO.h"
#include "Server/DAO/IDiscardFeedbackQuestionDAO.h"
#include "Server/DAO/IFeedbackDAO.h"
#include "Server/DAO/IFoodItemAttributeDAO.h"
#include "Server/DAO/IFoodItemDAO.h"
#include "Server/DAO/IReviewDAO.h"
#include <memory>

namespace Service {

using DAO::IFeedbackDAO;
using DAO::IFoodItemDAO;
using DAO::IReviewDAO;
using DTO::Feedback;
using DTO::FoodItem;
using DTO::Review;

class FoodItemService {
  std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO;
  std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO;
  std::shared_ptr<DAO::IReviewDAO> reviewDAO;
  std::shared_ptr<DAO::IFoodItemAttributeDAO> foodItemAttributeDAO;
  std::shared_ptr<DAO::IDiscardFeedbackQuestionDAO> discardFeedbackQuestionDAO;
  std::shared_ptr<DAO::IDiscardFeedbackAnswerDAO> discardFeedbackAnswerDAO;
  std::shared_ptr<DAO::AttributeDAO> attributeDAO;

public:
  FoodItemService(
      std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
      std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO,
      std::shared_ptr<DAO::IReviewDAO> reviewDAO,
      std::shared_ptr<DAO::IFoodItemAttributeDAO> foodItemAttributeDAO,
      std::shared_ptr<DAO::IDiscardFeedbackQuestionDAO>
          discardFeedbackQuestionDAO,
      std::shared_ptr<DAO::IDiscardFeedbackAnswerDAO> discardFeedbackAnswerDAO,
      std::shared_ptr<DAO::AttributeDAO> attributeDAO);

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
  bool addAttribute(std::string attributeName);
  bool removeAttributeFromFoodItem(uint64_t foodItemId, uint64_t attributeId);
  bool addAttributesToFoodItem(std::vector<uint64_t> attributeIds,
                               uint64_t foodItemId);
  std::vector<DTO::FoodItem>
  getFoodItemsWithGivenAttributes(uint64_t attributeId);
  std::vector<std::pair<uint64_t, std::string>>
  getAttributeDetails(const std::vector<uint64_t> &attributeId);

  bool addDiscardFeedbackQuestion(std::string question, uint64_t foodItemId);
  bool addDiscardFeedbackAnswer(std::string answer, uint64_t userId,
                                uint64_t questionId);
  std::vector<DTO::DiscardFeedbackQuestion>
  getQuestionByFoodItemId(uint64_t foodItemId);

  std::vector<DTO::DiscardFeedbackAnswer>
  getAnswerByQuestionId(uint64_t questionId);
  std::vector<DTO::FoodItem> getDiscardedFoodItems();
};

}; // namespace Service