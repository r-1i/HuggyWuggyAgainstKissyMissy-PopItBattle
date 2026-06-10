#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "IGameState.h"
#include "../combat/BattleConfig.h"
#include "../combat/BattleRoom.h"
#include "../items/Item.h"
#include "../ui/Button.h"

class GameStateManager;
class GameWorld;

class RoomState : public IGameState {
 public:
  using FinishCallback = std::function<void(
      int coinsEarned, std::vector<std::unique_ptr<Item>> loot, bool wiped)>;

  RoomState(GameStateManager& gsm, GameWorld& gameWorld,
            const BattleConfigEntry& config, int roomIndex, RoomKind kind,
            bool& isManualMode, FinishCallback onFinished);

  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;

 private:
  void refreshButtonLabel();

  GameStateManager& gsm_;
  bool& isManualMode_;
  FinishCallback onFinished_;

  BattleRoom battleRoom_;
  Button btnAutoManual_;
  sf::Font font_;
  sf::Text roomLabel_;
  bool finished_ = false;
};
