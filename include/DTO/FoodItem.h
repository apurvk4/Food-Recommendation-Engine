#pragma once

#include "FoodItemType.h"
#include "SerializableTypes/Double.h"
#include "SerializableTypes/SString.h"
#include "SerializableTypes/U64.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

namespace DTO {

struct FoodItem : public Serializable {
  U64 foodItemId;
  Double price;
  bool availabilityStatus;
  U64 foodItemTypeId;
  SString itemName;
  FoodItem()
      : foodItemId(0), price(0), availabilityStatus(false), foodItemTypeId(0) {}
  ~FoodItem() {}

  FoodItem(U64 foodItemId, Double price, bool availabilityStatus,
           U64 foodItemTypeId, SString itemName)
      : foodItemId(foodItemId), price(price),
        availabilityStatus(availabilityStatus), foodItemTypeId(foodItemTypeId),
        itemName(itemName) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> serialized;
    std::vector<unsigned char> foodItemIdSerialized = foodItemId.serialize();
    std::vector<unsigned char> priceSerialized = price.serialize();
    serialized.insert(serialized.end(), foodItemIdSerialized.begin(),
                      foodItemIdSerialized.end());
    serialized.insert(serialized.end(), priceSerialized.begin(),
                      priceSerialized.end());
    serialized.push_back(availabilityStatus);
    std::vector<unsigned char> foodItemTypeIdSerialized =
        foodItemTypeId.serialize();
    serialized.insert(serialized.end(), foodItemTypeIdSerialized.begin(),
                      foodItemTypeIdSerialized.end());

    std::vector<unsigned char> itemNameSer = itemName.serialize();
    serialized.insert(serialized.end(), itemNameSer.begin(), itemNameSer.end());
    return serialized;
  }

  uint64_t deserialize(const std::vector<unsigned char> &data) override {
    uint64_t bytesRead = 0;
    std::vector<unsigned char> foodItemIdData(data.begin(), data.end());
    bytesRead += foodItemId.deserialize(foodItemIdData);
    std::vector<unsigned char> priceData(data.begin() + bytesRead, data.end());
    bytesRead += price.deserialize(priceData);
    availabilityStatus = data[bytesRead];
    bytesRead += 1;
    std::vector<unsigned char> foodItemTypeIdData(data.begin() + bytesRead,
                                                  data.end());
    bytesRead += foodItemTypeId.deserialize(foodItemTypeIdData);
    bytesRead += itemName.deserialize(
        std::vector<unsigned char>(data.begin() + bytesRead, data.end()));
    return bytesRead;
  }
  size_t getSize() override {
    std::cout << "id : " << foodItemId.getSize() << std::endl;
    std::cout << "price : " << price.getSize() << std::endl;
    std::cout << "availability : " << 1 << std::endl;
    std::cout << "type : " << foodItemTypeId.getSize() << std::endl;
    std::cout << "name : " << itemName.getSize() << std::endl;
    return foodItemId.getSize() + price.getSize() + 1 +
           foodItemTypeId.getSize() + itemName.getSize();
  }
};
}; // namespace DTO