#include "EquipSlot.h"

#include <algorithm>

EquipSlot::EquipSlot(sf::Vector2f pos, float size, const std::string& iconPath)
    : size_(size),
      bgSprite_(bgTexture_),
      itemSprite_(itemTexture_),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      label_(font_, "", 14U) {
  bgTexture_.loadFromFile(iconPath);
  bgSprite_.setTexture(bgTexture_, true);
  sf::Vector2u sz = bgTexture_.getSize();
  bgSprite_.setScale({size_ / sz.x, size_ / sz.y});
  bgSprite_.setPosition(pos);

  bounds_ = bgSprite_.getGlobalBounds();

  label_.setFillColor(sf::Color::White);
}

void EquipSlot::setItem(const Item* item) {
  hasItem_ = (item != nullptr);
  hasItemSprite_ = false;
  label_.setString("");

  if (!hasItem_) return;

  sf::Vector2f pos = bgSprite_.getPosition();

  if (!item->getSpritePath().empty() &&
      itemTexture_.loadFromFile(item->getSpritePath())) {
    itemSprite_.setTexture(itemTexture_, true);
    sf::Vector2u sz = itemTexture_.getSize();
    float inner = size_ * 0.65f;
    float scale = inner / static_cast<float>(std::max(sz.x, sz.y));
    float spriteW = sz.x * scale;
    float spriteH = sz.y * scale;
    itemSprite_.setScale({scale, scale});
    itemSprite_.setPosition({pos.x + (size_ - spriteW) / 2.f,
                             pos.y + (size_ - spriteH) / 2.f});
    hasItemSprite_ = true;
  } else {
    label_.setString(item->getName().substr(0, 4));
    label_.setPosition(
        {pos.x + size_ / 2.f - label_.getLocalBounds().size.x / 2.f,
         pos.y + size_ / 2.f - label_.getLocalBounds().size.y / 2.f - 3.f});
  }
}

void EquipSlot::handleInput(const sf::Event& event) {
  if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (click->button == sf::Mouse::Button::Left) {
      if (bounds_.contains(sf::Vector2f(click->position))) {
        if (onClick) onClick();
      }
    }
  }
}

void EquipSlot::render(sf::RenderWindow& window) {
  window.draw(bgSprite_);
  if (hasItem_ && hasItemSprite_) window.draw(itemSprite_);
  if (!label_.getString().isEmpty()) window.draw(label_);
}
