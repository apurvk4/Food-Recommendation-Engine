#pragma once

#include "Attribute.h"
#include "SerializableTypes/SerializableTypes.h"

namespace DAO {

class IAttributeDAO {
public:
  virtual U64 addAttribute(SString attributeName) = 0;
  virtual bool removeAttribute(U64 attributeId) = 0;
  virtual std::vector<DTO::Attribute> getAllAttributes() = 0;
  virtual ~IAttributeDAO() = default;
};

}; // namespace DAO