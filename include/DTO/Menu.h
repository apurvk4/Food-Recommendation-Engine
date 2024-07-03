#pragma once

#include "FoodItem.h"
#include "MenuItem.h"
#include <cstdint>
#include <string>
#include <vector>

namespace DTO {

struct Menu : public Serializable {
  U64 menuId;
  SString menuName;
  U64 categoryId;
  bool isSurvey;
  SString date;
  Menu() = default;
  Menu(U64 menuId, SString menuName, U64 categoryId, bool isSurvey,
       SString date)
      : menuId(menuId), menuName(menuName), categoryId(categoryId),
        isSurvey(isSurvey), date(date) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> menuIdSerialized = menuId.serialize();
    std::vector<unsigned char> menuNameSerialized = menuName.serialize();
    std::vector<unsigned char> categoryIdSerialized = categoryId.serialize();
    std::vector<unsigned char> isSurveySerialized;
    isSurveySerialized.push_back(isSurvey);
    std::vector<unsigned char> dateSerialized = date.serialize();
    serialized.insert(serialized.end(), menuIdSerialized.begin(),
                      menuIdSerialized.end());
    serialized.insert(serialized.end(), menuNameSerialized.begin(),
                      menuNameSerialized.end());
    serialized.insert(serialized.end(), categoryIdSerialized.begin(),
                      categoryIdSerialized.end());
    serialized.insert(serialized.end(), isSurveySerialized.begin(),
                      isSurveySerialized.end());
    serialized.insert(serialized.end(), dateSerialized.begin(),
                      dateSerialized.end());
    return serialized;
  }
  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    uint64_t bytesRead = 0;
    bytesRead += menuId.deserialize(data);

    bytesRead += menuName.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));

    bytesRead += categoryId.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));

    isSurvey = data[bytesRead];
    bytesRead++;

    bytesRead += date.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() override {
    return menuId.getSize() + menuName.getSize() + categoryId.getSize() + 1 +
           date.getSize();
  }
};

}; // namespace DTO