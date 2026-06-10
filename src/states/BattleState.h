#pragma once

#include <memory>
#include <vector>

#include "IGameState.h"
#include "../combat/BattleConfig.h"
#include "../items/Item.h"

class GameStateManager;
class GameWorld;

class BattleState : public IGameState {
 public:
  BattleState(GameStateManager& gsm, GameWorld& gameWorld);

  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;

 private:
  static constexpr int kTotalRooms = 100;

  RoomKind roomKindForIndex(int roomIndex) const;
  void pushNextRoom();

  GameStateManager& gsm_;
  GameWorld& gameWorld_;

  int currentRoomIndex_ = 1;
  int coinsEarned_ = 0;
  std::vector<std::unique_ptr<Item>> lootEarned_;
  bool isManualMode_ = false;
  bool lastWiped_ = false;
  bool runFinished_ = false;
  bool started_ = false;
};
