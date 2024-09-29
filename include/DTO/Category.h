#pragma once

#include <stdexcept>
#include <string>
#include <cstdint>

namespace DTO {

enum class Category : uint64_t {
  Breakfast = 1,
  Lunch = 2,
  Dinner = 3,
};

inline std::string CategoryToString(Category category) {
  switch (category) {
  case Category::Breakfast:
    return "Breakfast";
  case Category::Lunch:
    return "Lunch";
  case Category::Dinner:
    return "Dinner";
  default:
    throw std::invalid_argument("Invalid category");
  }
}

inline Category StringToCategory(const std::string &category) {
  if (category == "Breakfast") {
    return Category::Breakfast;
  } else if (category == "Lunch") {
    return Category::Lunch;
  } else if (category == "Dinner") {
    return Category::Dinner;
  } else {
    throw std::invalid_argument("Invalid category");
  }
}
}; // namespace DTO