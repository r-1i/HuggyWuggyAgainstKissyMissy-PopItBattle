#pragma once
#include "AutoTargetSelector.h"
#include "ITargetSelector.h"

class ManualTargetSelector : public ITargetSelector {
 public:
  void setSelected(CombatEntity* target) { selected_ = target; }
  CombatEntity* getSelected() const { return selected_; }

  CombatEntity* selectTarget(const std::vector<CombatEntity*>& allies,
                              const std::vector<CombatEntity*>& enemies) override;

 private:
  CombatEntity* selected_ = nullptr;
  AutoTargetSelector fallback_;
};
