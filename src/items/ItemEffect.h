#pragma once

class Hero;
class CombatEntity;

class ItemEffect {
 public:
  virtual ~ItemEffect() = default;

  virtual void onEquip(Hero& hero) = 0;
  virtual void onUnequip(Hero& hero) = 0;
  virtual void onAttack(CombatEntity& attacker, CombatEntity& target) {}
  virtual void onTakeDamage(CombatEntity& owner, int& damageAmount) {}
  virtual int getValue() const { return 0; }
};
