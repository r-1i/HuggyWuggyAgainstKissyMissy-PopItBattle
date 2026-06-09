#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

#include "items/Item.h"

class EquipSlot {
 public:
  EquipSlot(sf::Vector2f pos, float size, const std::string& iconPath);
  void setItem(const Item* item);
  void handleInput(const sf::Event& event);
  void render(sf::RenderWindow& window);

  std::function<void()> onClick;

 private:
  float size_;
  sf::Texture bgTexture_;
  sf::Sprite bgSprite_;
  sf::Texture itemTexture_;
  sf::Sprite itemSprite_;
  sf::Font font_;
  sf::Text label_;
  sf::FloatRect bounds_;
  bool hasItem_ = false;
  bool hasItemSprite_ = false;
};
