#pragma once

#include "Category.h"
#include "DTO/Menu.h"
#include <vector>

namespace DAO {

using DTO::Menu;

class IMenuDAO {
public:
  virtual uint64_t addMenu(Menu menu) = 0;
  virtual bool deleteMenu(uint64_t menuId) = 0;
  virtual bool updateMenu(Menu menu) = 0;
  virtual Menu getMenu(uint64_t menuId) = 0;
  virtual std::vector<Menu> getAllMenus() = 0;
  virtual std::vector<Menu> getMenusByType(DTO::Category category) = 0;
  virtual uint64_t getMenuType(uint64_t menuId) = 0;
  virtual std::vector<DTO::Menu> getMenuByDate(std::string date) = 0;
  virtual std::vector<DTO::Menu> getMenuByCategory(DTO::Category category) = 0;
  virtual std::vector<DTO::Menu> getSurveyMenu(std::string date) = 0;
  virtual std::vector<DTO::Menu> getSurveyMenu(std::string date,
                                               DTO::Category category) = 0;
  virtual ~IMenuDAO() = default;
};

}; // namespace DAO