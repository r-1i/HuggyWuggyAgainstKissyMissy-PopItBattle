#pragma once

#include <memory>
#include <string>

#include "ItemEffect.h"

class Hero;
class CombatEntity;

class Item {
 public:
  Item(std::string name, int price, std::unique_ptr<ItemEffect> effect,
       std::string spritePath = "", std::string type = "");

  const std::string& getName() const;
  const std::string& getSpritePath() const;
  const std::string& getType() const;
  int getPrice() const;
  int getEffectValue() const;

  void onEquip(Hero& hero);
  void onUnequip(Hero& hero);
  void onAttack(CombatEntity& attacker, CombatEntity& target);
  void onTakeDamage(CombatEntity& owner, int& damageAmount);

 private:
  std::string name_;
  std::string spritePath_;
  std::string type_;
  int price_;
  std::unique_ptr<ItemEffect> effect_;
};
