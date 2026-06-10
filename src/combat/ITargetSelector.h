#pragma once
#include <vector>

class CombatEntity;

class ITargetSelector {
 public:
  virtual ~ITargetSelector() = default;
  virtual CombatEntity* selectTarget(const std::vector<CombatEntity*>& allies,
                                      const std::vector<CombatEntity*>& enemies) = 0;
};
