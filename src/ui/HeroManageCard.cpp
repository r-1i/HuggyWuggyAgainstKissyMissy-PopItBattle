#include "HeroManageCard.h"

#include <algorithm>

HeroManageCard::HeroManageCard(sf::Vector2f pos, Hero& hero,
                               std::function<void()> onSell,
                               std::function<void(bool isWeapon)> onSlotClick)
    : hero_(hero),
      labelSprite_(labelTexture_),
      iconFrameSprite_(iconFrameTexture_),
      heroSprite_(heroTexture_),
      sellSprite_(sellTexture_),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      nameText_(font_, hero.name, 22U),
      priceText_(font_, std::to_string(hero.price / 2), 18U),
      atkText_(font_, "ATK: -", 12U),
      defText_(font_, "DEF: -", 12U),
      onSell_(onSell),
      weaponSlot_({pos.x + 440.f, pos.y + 17.f}, 75.f,
                  "assets/sprites/ui/manage_hero_attack_icon.png"),
      armorSlot_({pos.x + 540.f, pos.y + 17.f}, 75.f,
                 "assets/sprites/ui/manage_hero_defence_icon.png") {
  static constexpr float kCardW = 630.f;
  static constexpr float kCardH = 110.f;
  static constexpr float kPad = 10.f;
  static constexpr float kIconSize = 90.f;
  static constexpr float kRightX = kPad + kIconSize + kPad;  // 110
  static constexpr float kRightW = kCardW - kRightX - kPad;  // 510
  static constexpr float kSellH = 48.f;
  static constexpr float kSellY = kCardH - kPad - kSellH;  // 52

  // Card background
  labelTexture_.loadFromFile("assets/sprites/ui/manage_hero_label.png");
  labelSprite_.setTexture(labelTexture_, true);
  sf::Vector2u sz = labelTexture_.getSize();
  labelSprite_.setScale({kCardW / sz.x, kCardH / sz.y});
  labelSprite_.setPosition(pos);

  // Icon frame
  iconFrameTexture_.loadFromFile("assets/sprites/ui/manage_hero_icon.png");
  iconFrameSprite_.setTexture(iconFrameTexture_, true);
  sf::Vector2u fsz = iconFrameTexture_.getSize();
  iconFrameSprite_.setScale({kIconSize / fsz.x, kIconSize / fsz.y});
  iconFrameSprite_.setPosition({pos.x + kPad, pos.y + kPad});

  // Hero portrait inside frame
  if (!hero.iconPath.empty() && heroTexture_.loadFromFile(hero.iconPath)) {
    heroSprite_.setTexture(heroTexture_, true);
    sf::Vector2u hsz = heroTexture_.getSize();
    float inner = kIconSize - kPad * 2;
    float scale = inner / static_cast<float>(std::max(hsz.x, hsz.y));
    heroSprite_.setScale({scale, scale});
    heroSprite_.setPosition({pos.x + kPad * 2, pos.y + kPad * 2});
  }

  // Sell button
  sellTexture_.loadFromFile("assets/sprites/ui/manage_hero_sell.png");
  sellSprite_.setTexture(sellTexture_, true);
  sellSprite_.setPosition({pos.x + kRightX, pos.y + kSellY});
  sellBounds_ = sellSprite_.getGlobalBounds();

  // Name
  nameText_.setFillColor(sf::Color::White);
  nameText_.setPosition({pos.x + kRightX, pos.y + kPad});

  // Price text: slightly right of center, dark brown
  priceText_.setFillColor(sf::Color(60, 20, 5));
  priceText_.setPosition(
      {sellBounds_.position.x + sellBounds_.size.x / 2.f + 10.f,
       sellBounds_.position.y + sellBounds_.size.y / 2.f -
           priceText_.getLocalBounds().size.y / 2.f - 4.f});

  // Stat labels (centered under each slot)
  atkCenterX_ = pos.x + 440.f + 75.f / 2.f;
  defCenterX_ = pos.x + 540.f + 75.f / 2.f;
  statTextY_  = pos.y + 94.f;
  atkText_.setFillColor(sf::Color::White);
  defText_.setFillColor(sf::Color::White);

  // Equip slot callbacks
  weaponSlot_.onClick = [onSlotClick]() { onSlotClick(true); };
  armorSlot_.onClick = [onSlotClick]() { onSlotClick(false); };

  refreshSlots();
}

void HeroManageCard::refreshSlots() {
  weaponSlot_.setItem(hero_.getWeapon());
  armorSlot_.setItem(hero_.getArmor());

  const Item* w = hero_.getWeapon();
  atkText_.setString(w ? "ATK: +" + std::to_string(w->getEffectValue()) : "ATK: -");
  atkText_.setPosition(
      {atkCenterX_ - atkText_.getLocalBounds().size.x / 2.f, statTextY_});

  const Item* a = hero_.getArmor();
  defText_.setString(a ? "DEF: +" + std::to_string(a->getEffectValue()) : "DEF: -");
  defText_.setPosition(
      {defCenterX_ - defText_.getLocalBounds().size.x / 2.f, statTextY_});
}

void HeroManageCard::handleInput(const sf::Event& event) {
  if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (click->button == sf::Mouse::Button::Left) {
      if (sellBounds_.contains(sf::Vector2f(click->position))) {
        if (onSell_) onSell_();
        return;
      }
    }
  }
  weaponSlot_.handleInput(event);
  armorSlot_.handleInput(event);
}

void HeroManageCard::render(sf::RenderWindow& window) {
  window.draw(labelSprite_);
  window.draw(iconFrameSprite_);
  window.draw(heroSprite_);
  window.draw(nameText_);
  window.draw(sellSprite_);
  window.draw(priceText_);
  weaponSlot_.render(window);
  armorSlot_.render(window);
  window.draw(atkText_);
  window.draw(defText_);
}
