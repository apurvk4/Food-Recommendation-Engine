#pragma once

#include "IReviewDAO.h"
#include "Server/DbConnection.h"
#include <memory>


namespace DAO {

using DAO::IReviewDAO;
using DTO::Review;

class ReviewDAO : public IReviewDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  ReviewDAO();
  bool addReview(Review) override;
  Review getReviewById(uint64_t ReviewId) override;
  std::vector<Review> getReviewsByUserId(uint64_t userId) override;
  std::vector<Review> getReviewsByFoodItemId(uint64_t foodItemId) override;
  std::vector<Review> getAllReviews() override;
};

}; // namespace DAO