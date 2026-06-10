#include "Item.h"

Item::Item(std::string name, int price, std::unique_ptr<ItemEffect> effect,
           std::string spritePath, std::string type)
    : name_(std::move(name)),
      spritePath_(std::move(spritePath)),
      type_(std::move(type)),
      price_(price),
      effect_(std::move(effect)) {}

const std::string& Item::getName() const { return name_; }
const std::string& Item::getSpritePath() const { return spritePath_; }
const std::string& Item::getType() const { return type_; }
int Item::getPrice() const { return price_; }
int Item::getEffectValue() const { return effect_ ? effect_->getValue() : 0; }

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
