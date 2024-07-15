#pragma once

#include "DTO/Review.h"
#include <vector>

namespace DAO {

class IReviewDAO {
public:
  virtual bool addReview(DTO::Review) = 0;
  virtual DTO::Review getReviewById(uint64_t ReviewId) = 0;
  virtual std::vector<DTO::Review> getReviewsByUserId(uint64_t userId) = 0;
  virtual std::vector<DTO::Review>
  getReviewsByFoodItemId(uint64_t foodItemId) = 0;
  virtual std::vector<DTO::Review> getAllReviews() = 0;
  virtual ~IReviewDAO() = default;
};

}; // namespace DAO