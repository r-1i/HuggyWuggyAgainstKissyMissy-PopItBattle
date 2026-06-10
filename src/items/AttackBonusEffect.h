#pragma once

#include "ItemEffect.h"

class AttackBonusEffect : public ItemEffect {
 public:
  explicit AttackBonusEffect(int bonusDamage);

  void onEquip(Hero& hero) override;
  void onUnequip(Hero& hero) override;
  int getValue() const override { return bonusDamage_; }

 private:
  int bonusDamage_;
};
