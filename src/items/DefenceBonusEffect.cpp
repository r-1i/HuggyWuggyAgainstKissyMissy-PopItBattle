#include "DefenceBonusEffect.h"

#include "../entities/Hero.h"

DefenceBonusEffect::DefenceBonusEffect(int bonusDefence) : bonusDefence_(bonusDefence) {}

void DefenceBonusEffect::onEquip(Hero& hero) {
  hero.armor += bonusDefence_;
}

void DefenceBonusEffect::onUnequip(Hero& hero) {
  hero.armor -= bonusDefence_;
}
