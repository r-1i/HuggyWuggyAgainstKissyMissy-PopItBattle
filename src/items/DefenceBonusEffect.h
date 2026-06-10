#pragma once

#include "ItemEffect.h"

class DefenceBonusEffect : public ItemEffect {
 public:
  explicit DefenceBonusEffect(int bonusDefence);

  void onEquip(Hero& hero) override;
  void onUnequip(Hero& hero) override;
  int getValue() const override { return bonusDefence_; }

 private:
  int bonusDefence_;
};
