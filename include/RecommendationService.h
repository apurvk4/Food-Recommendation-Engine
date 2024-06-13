#pragma once

#include "IFeedbackDAO.h"
#include "Menu.h"
#include <MenuType.h>
#include <memory>

class RecommendationService {
public:
  RecommendationService(std::shared_ptr<IFeedbackDAO> feedbackDAO);
  Menu getRecommendedMenu(MenuType);
};