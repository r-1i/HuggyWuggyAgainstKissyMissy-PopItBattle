#include "BattleState.h"

#include "BattleResultState.h"
#include "RoomState.h"
#include "../core/GameStateManager.h"
#include "../core/GameWorld.h"

BattleState::BattleState(GameStateManager& gsm, GameWorld& gameWorld)
    : gsm_(gsm), gameWorld_(gameWorld) {
  gameWorld_.startNewRun();
}

RoomKind BattleState::roomKindForIndex(int roomIndex) const {
  if (roomIndex % 10 == 0) return RoomKind::Boss;
  if (roomIndex % 5 == 0) return RoomKind::Chest;
  return RoomKind::Normal;
}

void BattleState::pushNextRoom() {
  RoomKind kind = roomKindForIndex(currentRoomIndex_);
  const BattleConfigEntry& config =
      gameWorld_.getAssetManager().getBattleConfigForRoom(currentRoomIndex_);

  gsm_.pushState(std::make_unique<RoomState>(
      gsm_, gameWorld_, config, currentRoomIndex_, kind, isManualMode_,
      [this](int coins, std::vector<std::unique_ptr<Item>> loot, bool wiped) {
        coinsEarned_ += coins;
        for (auto& item : loot) lootEarned_.push_back(std::move(item));
        lastWiped_ = wiped;
      }));
}

void BattleState::handleInput(const sf::Event& event) {}

void BattleState::update(float dt) {
  if (!started_) {
    started_ = true;
    pushNextRoom();
  }
}

void BattleState::onResume() {
  if (runFinished_) {
    gsm_.popState();
    return;
  }

  if (lastWiped_) {
    runFinished_ = true;
    gsm_.pushState(std::make_unique<BattleResultState>(
        gsm_, gameWorld_, false, coinsEarned_, std::move(lootEarned_)));
    return;
  }

  currentRoomIndex_++;
  if (currentRoomIndex_ > kTotalRooms) {
    runFinished_ = true;
    gsm_.pushState(std::make_unique<BattleResultState>(
        gsm_, gameWorld_, true, coinsEarned_, std::move(lootEarned_)));
    return;
  }

  pushNextRoom();
}

void BattleState::render(sf::RenderWindow& window) {}

const bool BattleState::shouldRenderBelow() const { return false; }

const bool BattleState::shouldUpdateBelow() const { return false; }
