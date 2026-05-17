#pragma once

#include "Button.h"
#include "entities/HeroTemplate.h"

class HeroRecruitCard : public Button {
 public:
  HeroRecruitCard(const sf::Vector2f& position,
                  const std::string& textureFileName,
                  HeroTemplate heroRecruitCard, std::function<void()> onClick);
  HeroTemplate heroTemplate;
};
