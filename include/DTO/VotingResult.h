#pragma once

#include "SerializableTypes/ProtocolDefinitions.h"

namespace DTO {

struct VotingResult : public Serializable {
  U64 foodItemId;
  U64 numberOfVotes;
  U64 numberOfPositiveVotes;
  U64 numberOfNegativeVotes;
  SString foodItemName;

public:
  VotingResult() = default;
  VotingResult(U64 foodItemId, U64 numberOfVotes, U64 numberOfPositiveVotes,
               U64 numberOfNegativeVotes, SString foodItemName)
      : foodItemId(foodItemId), numberOfVotes(numberOfVotes),
        numberOfPositiveVotes(numberOfPositiveVotes),
        numberOfNegativeVotes(numberOfNegativeVotes),
        foodItemName(foodItemName) {}

  std::vector<unsigned char> serialize() override {
    std::vector<unsigned char> bytes;
    std::vector<unsigned char> foodItemIdBytes = foodItemId.serialize();
    std::vector<unsigned char> numberOfVotesBytes = numberOfVotes.serialize();
    std::vector<unsigned char> numberOfPositiveVotesBytes =
        numberOfPositiveVotes.serialize();
    std::vector<unsigned char> numberOfNegativeVotesBytes =
        numberOfNegativeVotes.serialize();
    std::vector<unsigned char> foodItemNameBytes = foodItemName.serialize();
    bytes.insert(bytes.end(), foodItemIdBytes.begin(), foodItemIdBytes.end());
    bytes.insert(bytes.end(), numberOfVotesBytes.begin(),
                 numberOfVotesBytes.end());
    bytes.insert(bytes.end(), numberOfPositiveVotesBytes.begin(),
                 numberOfPositiveVotesBytes.end());
    bytes.insert(bytes.end(), numberOfNegativeVotesBytes.begin(),
                 numberOfNegativeVotesBytes.end());
    bytes.insert(bytes.end(), foodItemNameBytes.begin(),
                 foodItemNameBytes.end());
    return bytes;
  }
  uint64_t deserialize(const std::vector<unsigned char> &bytes) override {
    std::size_t bytesRead = 0;
    bytesRead += foodItemId.deserialize(
        std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
    bytesRead += numberOfVotes.deserialize(
        std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
    bytesRead += numberOfPositiveVotes.deserialize(
        std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
    bytesRead += numberOfNegativeVotes.deserialize(
        std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
    bytesRead += foodItemName.deserialize(
        std::vector<unsigned char>(bytes.begin() + bytesRead, bytes.end()));
    return bytesRead;
  }
  std::size_t getSize() override {
    return foodItemName.getSize() + numberOfVotes.getSize() +
           numberOfPositiveVotes.getSize() + numberOfNegativeVotes.getSize() +
           foodItemId.getSize();
  }
};

}; // namespace DTO