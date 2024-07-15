#include "Server/Service/FoodItemService.h"
#include "DiscardFeedbackQuestion.h"
#include "FoodItem.h"
#include <cstdint>

using Service::FoodItemService;

FoodItemService::FoodItemService(
    std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
    std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO,
    std::shared_ptr<DAO::IReviewDAO> reviewDAO,
    std::shared_ptr<DAO::IFoodItemAttributeDAO> foodItemAttributeDAO,
    std::shared_ptr<DAO::IDiscardFeedbackQuestionDAO>
        discardFeedbackQuestionDAO,
    std::shared_ptr<DAO::IDiscardFeedbackAnswerDAO> discardFeedbackAnswerDAO)
    : foodItemDAO(foodItemDAO), feedbackDAO(feedbackDAO), reviewDAO(reviewDAO),
      foodItemAttributeDAO{foodItemAttributeDAO},
      discardFeedbackQuestionDAO{discardFeedbackQuestionDAO},
      discardFeedbackAnswerDAO(discardFeedbackAnswerDAO) {}

bool FoodItemService::addFoodItem(FoodItem foodItem) {
  return foodItemDAO->addFoodItem(foodItem);
}

bool FoodItemService::updateFoodItem(FoodItem foodItem) {
  return foodItemDAO->updateFoodItem(foodItem);
}

bool FoodItemService::deleteFoodItem(uint64_t foodItemId) {
  return foodItemDAO->deleteFoodItem(foodItemId);
}

DTO::FoodItem FoodItemService::getFoodItemById(uint64_t foodItemId) {
  return foodItemDAO->getFoodItemById(foodItemId);
}

std::vector<DTO::FoodItem> FoodItemService::getAllFoodItems() {
  return foodItemDAO->getAllFoodItems();
}

std::vector<DTO::FoodItem>
FoodItemService::getFoodItemsByCategory(DTO::Category category) {
  return foodItemDAO->getFoodItemsByType(category);
}

bool FoodItemService::addFeedback(DTO::Feedback feedback) {
  return feedbackDAO->addFeedback(feedback);
}

std::vector<DTO::Feedback> FoodItemService::getAllFeedback(uint64_t foodItemId,
                                                           std::string date) {
  return feedbackDAO->getFeedbackByFoodItemId(foodItemId, date);
}

std::vector<DTO::Feedback>
FoodItemService::getFeedbackByUserId(uint64_t userId) {
  return feedbackDAO->getFeedbackByUserId(userId);
}

std::vector<DTO::Feedback>
FoodItemService::getFeedbackByUserId(uint64_t userId, std::string date) {
  return feedbackDAO->getFeedbackByUserId(userId, date);
}

bool FoodItemService::addReview(DTO::Review review) {
  return reviewDAO->addReview(review);
}

std::vector<DTO::Review> FoodItemService::getAllReviews(uint64_t foodItemId) {
  return reviewDAO->getReviewsByFoodItemId(foodItemId);
}

std::vector<DTO::Review> FoodItemService::getAllReviews(uint64_t foodItemId,
                                                        std::string date) {
  auto reviews = getAllReviews(foodItemId);
  std::vector<DTO::Review> filteredReviews;
  for (auto review : reviews) {
    if (review.date == date) {
      filteredReviews.push_back(review);
    }
  }
  return filteredReviews;
}

std::vector<DTO::Review> FoodItemService::getReviewsByUserId(uint64_t userId) {
  return reviewDAO->getReviewsByUserId(userId);
}

std::vector<DTO::Review>
FoodItemService::getReviewsByUserId(uint64_t userId, uint64_t foodItemId) {
  auto reviews = getReviewsByUserId(userId);
  std::vector<DTO::Review> filteredReviews;
  for (auto review : reviews) {
    if (review.foodItemId == (U64)foodItemId) {
      filteredReviews.push_back(review);
    }
  }
  return filteredReviews;
}

std::vector<DTO::Review>
FoodItemService::getReviewsByUserId(uint64_t userId, uint64_t foodItemId,
                                    std::string date) {
  auto reviews = getReviewsByUserId(userId, foodItemId);
  std::vector<DTO::Review> filteredReviews;
  for (auto review : reviews) {
    if (review.date == date) {
      filteredReviews.push_back(review);
    }
  }
  return filteredReviews;
}

bool FoodItemService::addAttributeToFoodItem(uint64_t foodItemId,
                                             uint64_t attributeId) {
  return foodItemAttributeDAO->addFoodItemAttribute(foodItemId, attributeId);
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getFoodItemAttributes(uint64_t foodItemId) {
  return foodItemAttributeDAO->getFoodItemAttributes(foodItemId);
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getAllAttributes() {
  return foodItemAttributeDAO->getAllAttributes();
}

bool FoodItemService::removeAttributeFromFoodItem(uint64_t foodItemId,
                                                  uint64_t attributeId) {
  return foodItemAttributeDAO->removeFoodItemAttribute(foodItemId, attributeId);
}

bool FoodItemService::addAttributesToFoodItem(
    std::vector<uint64_t> attributeIds, uint64_t foodItemId) {
  return foodItemAttributeDAO->addFoodItemAttribute(attributeIds, foodItemId);
}

std::vector<DTO::FoodItem>
FoodItemService::getFoodItemsWithGivenAttributes(uint64_t attributeId) {
  std::vector<uint64_t> foodItemIds =
      foodItemAttributeDAO->getFoodItemsWithGivenAttributes(attributeId);
  std::vector<DTO::FoodItem> foodItems;
  for (auto foodItemId : foodItemIds) {
    foodItems.push_back(foodItemDAO->getFoodItemById(foodItemId));
  }
  return foodItems;
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getAttributeDetails(const std::vector<uint64_t> &attributes) {
  auto allAttributes = foodItemAttributeDAO->getAllAttributes();
  std::vector<std::pair<uint64_t, std::string>> attributeDetails;
  for (auto attribute : attributes) {
    for (auto attr : allAttributes) {
      if (attr.first == attribute) {
        attributeDetails.push_back(attr);
      }
    }
  }
  return attributeDetails;
}

bool FoodItemService::addDiscardFeedbackQuestion(std::string question,
                                                 uint64_t foodItemId) {
  return discardFeedbackQuestionDAO->addQuestion(
      DTO::DiscardFeedbackQuestion(0, foodItemId, question));
}

bool FoodItemService::addDiscardFeedbackAnswer(std::string answer,
                                               uint64_t userId,
                                               uint64_t questionId) {
  return discardFeedbackAnswerDAO->addAnswer(
      DTO::DiscardFeedbackAnswer(0, questionId, userId, answer));
}

std::vector<DTO::DiscardFeedbackQuestion>
FoodItemService::getQuestionByFoodItemId(uint64_t foodItemId) {
  return discardFeedbackQuestionDAO->getQuestionsByFoodItemId(foodItemId);
}

std::vector<DTO::DiscardFeedbackAnswer>
FoodItemService::getAnswerByQuestionId(uint64_t questionId) {
  return discardFeedbackAnswerDAO->getAnswersByQuestionId(questionId);
}

std::vector<DTO::FoodItem> FoodItemService::getDiscardedFoodItems() {
  return foodItemDAO->getDiscardedFoodItems();
}