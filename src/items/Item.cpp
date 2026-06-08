#include "Item.h"

Item::Item(std::string name, int price, std::unique_ptr<ItemEffect> effect)
    : name_(std::move(name)), price_(price), effect_(std::move(effect)) {}

const std::string& Item::getName() const {
  return name_;
}

int Item::getPrice() const {
  return price_;
}

void Item::onEquip(Hero& hero) {
  if (effect_)
    effect_->onEquip(hero);
}

void Item::onUnequip(Hero& hero) {
  if (effect_)
    effect_->onUnequip(hero);
}

void Item::onAttack(CombatEntity& attacker, CombatEntity& target) {
  if (effect_)
    effect_->onAttack(attacker, target);
}

void Item::onTakeDamage(CombatEntity& owner, int& damageAmount) {
  if (effect_)
    effect_->onTakeDamage(owner, damageAmount);
}
