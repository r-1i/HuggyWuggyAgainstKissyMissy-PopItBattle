#pragma once

#include "ItemEffect.h"

class AttackBonusEffect : public ItemEffect {
 public:
  explicit AttackBonusEffect(int bonusDamage);

  void onEquip(Hero& hero) override;
  void onUnequip(Hero& hero) override;

 private:
  int bonusDamage_;
};
