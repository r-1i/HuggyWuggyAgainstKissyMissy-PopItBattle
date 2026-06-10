#include "ManualTargetSelector.h"

#include <algorithm>

#include "CombatEntity.h"

CombatEntity* ManualTargetSelector::selectTarget(
    const std::vector<CombatEntity*>& allies,
    const std::vector<CombatEntity*>& enemies) {
  if (selected_ && selected_->isAlive() &&
      std::find(enemies.begin(), enemies.end(), selected_) != enemies.end()) {
    return selected_;
  }

  return fallback_.selectTarget(allies, enemies);
}
