#pragma once
#include "ITargetSelector.h"

class AutoTargetSelector : public ITargetSelector {
 public:
  CombatEntity* selectTarget(const std::vector<CombatEntity*>& allies,
                              const std::vector<CombatEntity*>& enemies) override;
};
