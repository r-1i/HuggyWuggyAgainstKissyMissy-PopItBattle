#include "AutoTargetSelector.h"

#include <random>

#include "CombatEntity.h"

CombatEntity* AutoTargetSelector::selectTarget(
    const std::vector<CombatEntity*>& allies,
    const std::vector<CombatEntity*>& enemies) {
  std::vector<CombatEntity*> alive;
  for (auto* entity : enemies)
    if (entity->isAlive()) alive.push_back(entity);

  if (alive.empty()) return nullptr;

  static std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<size_t> dist(0, alive.size() - 1);
  return alive[dist(rng)];
}
