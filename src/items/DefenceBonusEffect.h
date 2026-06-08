#pragma once

#include "ItemEffect.h"

class DefenceBonusEffect : public ItemEffect {
 public:
  explicit DefenceBonusEffect(int bonusDefence);

  void onEquip(Hero& hero) override;
  void onUnequip(Hero& hero) override;

 private:
  int bonusDefence_;
};
