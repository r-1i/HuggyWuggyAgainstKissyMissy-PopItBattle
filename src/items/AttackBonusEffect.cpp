#include "AttackBonusEffect.h"

#include "../entities/Hero.h"

AttackBonusEffect::AttackBonusEffect(int bonusDamage)
    : bonusDamage_(bonusDamage) {}

void AttackBonusEffect::onEquip(Hero& hero) { hero.damage += bonusDamage_; }

void AttackBonusEffect::onUnequip(Hero& hero) { hero.damage -= bonusDamage_; }
