#pragma once

#include "Attribute.h"
#include "IAttributeDAO.h"
#include "Server/DbConnection.h"
#include <memory>

namespace DAO {

class AttributeDAO : public IAttributeDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  AttributeDAO();
  U64 addAttribute(SString attributeName) override;
  bool removeAttribute(U64 attributeId) override;
  std::vector<DTO::Attribute> getAllAttributes() override;
  ~AttributeDAO() = default;
};

}; // namespace DAO