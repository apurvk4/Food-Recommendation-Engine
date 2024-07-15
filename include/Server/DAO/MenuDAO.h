#pragma once

#include "Category.h"
#include "DTO/Menu.h"
#include "DTO/MenuItem.h"
#include "IMenuDAO.h"
#include "Server/DbConnection.h"
#include <cstdint>
#include <vector>


namespace DAO {

class MenuDAO : public IMenuDAO {
  std::shared_ptr<DbConnection> dbConnection;

public:
  MenuDAO();
  uint64_t addMenu(DTO::Menu menu) override;
  bool deleteMenu(uint64_t menuId) override;
  bool updateMenu(DTO::Menu menu) override;
  DTO::Menu getMenu(uint64_t menuId) override;
  std::vector<DTO::Menu> getAllMenus() override;
  uint64_t getMenuType(uint64_t menuId) override;
  std::vector<DTO::Menu> getMenusByType(DTO::Category category) override;
  std::vector<DTO::Menu> getMenuByDate(std::string date) override;
  std::vector<DTO::Menu> getMenuByCategory(DTO::Category category) override;
  std::vector<DTO::Menu> getSurveyMenu(std::string date) override;
  std::vector<DTO::Menu> getSurveyMenu(std::string date,
                                       DTO::Category category) override;
};

}; // namespace DAO