#include "Hero.h"

Hero::Hero(const HeroTemplate& heroTemplate)
    : name(heroTemplate.name),
      maxHealth(heroTemplate.maxHp),
      damage(heroTemplate.damage) {}

void Hero::equipWeapon(std::unique_ptr<Item> item) {
  equipedWeapon = std::move(item);
}

void Hero::equipArmor(std::unique_ptr<Item> item) {
  equipedArmor = std::move(item);
}
