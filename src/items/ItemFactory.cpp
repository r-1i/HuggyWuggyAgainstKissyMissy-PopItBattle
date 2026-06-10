#include "ItemFactory.h"

#include "AttackBonusEffect.h"
#include "DefenceBonusEffect.h"

std::unique_ptr<Item> makeItem(const ItemTemplate& t) {
  if (t.type == "weapon")
    return std::make_unique<Item>(t.name, t.price,
                                  std::make_unique<AttackBonusEffect>(t.effectValue),
                                  t.spritePath, t.type);
  return std::make_unique<Item>(t.name, t.price,
                                std::make_unique<DefenceBonusEffect>(t.effectValue),
                                t.spritePath, t.type);
}
