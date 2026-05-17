#include "HeroRecruitCard.h"

HeroRecruitCard::HeroRecruitCard(const sf::Vector2f& position,
                                 const std::string& textureFileName,
                                 HeroTemplate heroRecruitCard,
                                 std::function<void()> onClick)
    : Button("", position, {0, 0}, textureFileName, true),
      heroTemplate(heroRecruitCard) {
  this->onClick = onClick;
}
/*
  Button(const sf::String& text, const sf::Vector2f& position,
         const sf::Vector2f& size = kButtonSize,
         const std::string& textureFileName = std::string(kDefaultTexture),
         bool overrideSize = true);
*/
