#include "Service/FoodItemService.h"

using Service::FoodItemService;

FoodItemService::FoodItemService(
    std::shared_ptr<DAO::IFoodItemDAO> foodItemDAO,
    std::shared_ptr<DAO::IFeedbackDAO> feedbackDAO,
    std::shared_ptr<DAO::IReviewDAO> reviewDAO,
    std::shared_ptr<DAO::IFoodItemAttribute> foodItemAttributeDAO)
    : m_foodItemDAO(foodItemDAO), m_feedbackDAO(feedbackDAO),
      m_reviewDAO(reviewDAO), m_foodItemAttributeDAO{foodItemAttributeDAO} {}

bool FoodItemService::addFoodItem(FoodItem foodItem) {
  return m_foodItemDAO->addFoodItem(foodItem);
}

bool FoodItemService::updateFoodItem(FoodItem foodItem) {
  return m_foodItemDAO->updateFoodItem(foodItem);
}

bool FoodItemService::deleteFoodItem(uint64_t foodItemId) {
  return m_foodItemDAO->deleteFoodItem(foodItemId);
}

DTO::FoodItem FoodItemService::getFoodItemById(uint64_t foodItemId) {
  return m_foodItemDAO->getFoodItemById(foodItemId);
}

std::vector<DTO::FoodItem> FoodItemService::getAllFoodItems() {
  return m_foodItemDAO->getAllFoodItems();
}

std::vector<DTO::FoodItem>
FoodItemService::getFoodItemsByCategory(DTO::Category category) {
  return m_foodItemDAO->getFoodItemsByType(category);
}

bool FoodItemService::addFeedback(DTO::Feedback feedback) {
  return m_feedbackDAO->addFeedback(feedback);
}

std::vector<DTO::Feedback> FoodItemService::getAllFeedback(uint64_t foodItemId,
                                                           std::string date) {
  return m_feedbackDAO->getFeedbackByFoodItemId(foodItemId, date);
}

std::vector<DTO::Feedback>
FoodItemService::getFeedbackByUserId(uint64_t userId) {
  return m_feedbackDAO->getFeedbackByUserId(userId);
}

std::vector<DTO::Feedback>
FoodItemService::getFeedbackByUserId(uint64_t userId, std::string date) {
  return m_feedbackDAO->getFeedbackByUserId(userId, date);
}

bool FoodItemService::addReview(DTO::Review review) {
  return m_reviewDAO->addReview(review);
}

std::vector<DTO::Review> FoodItemService::getAllReviews(uint64_t foodItemId) {
  return m_reviewDAO->getReviewsByFoodItemId(foodItemId);
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
  return m_reviewDAO->getReviewsByUserId(userId);
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
  return m_foodItemAttributeDAO->addFoodItemAttribute(foodItemId, attributeId);
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getFoodItemAttributes(uint64_t foodItemId) {
  return m_foodItemAttributeDAO->getFoodItemAttributes(foodItemId);
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getAllAttributes() {
  return m_foodItemAttributeDAO->getAllAttributes();
}

bool FoodItemService::removeAttributeFromFoodItem(uint64_t foodItemId,
                                                  uint64_t attributeId) {
  return m_foodItemAttributeDAO->removeFoodItemAttribute(foodItemId,
                                                         attributeId);
}

bool FoodItemService::addAttributesToFoodItem(
    std::vector<uint64_t> attributeIds, uint64_t foodItemId) {
  return m_foodItemAttributeDAO->addFoodItemAttribute(attributeIds, foodItemId);
}

std::vector<DTO::FoodItem>
FoodItemService::getFoodItemsWithGivenAttributes(uint64_t attributeId) {
  std::vector<uint64_t> foodItemIds =
      m_foodItemAttributeDAO->getFoodItemsWithGivenAttributes(attributeId);
  std::vector<DTO::FoodItem> foodItems;
  for (auto foodItemId : foodItemIds) {
    foodItems.push_back(m_foodItemDAO->getFoodItemById(foodItemId));
  }
  return foodItems;
}

std::vector<std::pair<uint64_t, std::string>>
FoodItemService::getAttributeDetails(const std::vector<uint64_t> &attributes) {
  auto allAttributes = m_foodItemAttributeDAO->getAllAttributes();
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