#pragma once

#include <memory>
#include <string>

#include "ItemEffect.h"

class Hero;
class CombatEntity;

class Item {
 public:
  Item(std::string name, int price, std::unique_ptr<ItemEffect> effect);

  const std::string& getName() const;
  int getPrice() const;

  void onEquip(Hero& hero);
  void onUnequip(Hero& hero);
  void onAttack(CombatEntity& attacker, CombatEntity& target);
  void onTakeDamage(CombatEntity& owner, int& damageAmount);

 private:
  std::string name_;
  int price_;
  std::unique_ptr<ItemEffect> effect_;
};
