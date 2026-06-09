#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "entities/Hero.h"
#include "ui/EquipSlot.h"

class HeroManageCard {
 public:
  HeroManageCard(sf::Vector2f pos, Hero& hero, std::function<void()> onSell,
                 std::function<void(bool isWeapon)> onSlotClick);
  void refreshSlots();
  void handleInput(const sf::Event& event);
  void render(sf::RenderWindow& window);

 private:
  Hero& hero_;
  sf::Texture labelTexture_;
  sf::Sprite labelSprite_;
  sf::Texture iconFrameTexture_;
  sf::Sprite iconFrameSprite_;
  sf::Texture heroTexture_;
  sf::Sprite heroSprite_;
  sf::Texture sellTexture_;
  sf::Sprite sellSprite_;
  sf::FloatRect sellBounds_;
  sf::Font font_;
  sf::Text nameText_;
  sf::Text priceText_;
  sf::Text atkText_;
  sf::Text defText_;
  std::function<void()> onSell_;
  EquipSlot weaponSlot_;  // attack icon, centre-right
  EquipSlot armorSlot_;   // defence icon, far right
  float atkCenterX_ = 0.f;
  float defCenterX_ = 0.f;
  float statTextY_  = 0.f;
};
