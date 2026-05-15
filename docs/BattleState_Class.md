---
config:
  layout: elk
  theme: redux
title: BattleState (Class Diagram)
---
classDiagram
direction TB

class IGameState {
  <<interface>>
  +handleInput(sf::Event event) = 0 void
  +update(float dt) = 0 void
  +onResume() = 0 void
  +render(sf::RenderWindow& window) = 0 void
  +shouldRenderBelow() = 0 const bool
  +shouldUpdateBelow() = 0 const bool
}

class BattleState {
  -GameStateManager& gsm_
  -GameWorld& gameWorld_
  -int coinsAddedThisState_
  -int currentRoomIndex_
  -const int TOTAL_ROOMS
  -BattleRoom currentRoom_
  -bool isManualMode
  +handleInput(sf::Event event) override void
  +update(float dt) override void
  +onResume() override void
  +render(sf::RenderWindow& window) override void
}

class GameStateManager {
  -std::stack~std::unique_ptr~IGameState~~ states_
  +pushState(std::unique_ptr~IGameState~ state) void
  +popState() void
}

class GameWorld {
  +getCurrentParty() std::vector~Hero~&
  +addCoins(int amount) void
  +addItem(std::unique_ptr~Item~ item) void
  +finishRun(int goldFromRun, std::vector~std::unique_ptr~Item~~ loot) void
  +clearParty() void
  +getAssetManager() AssetManager&
}

class AssetManager {
  +getRoomTemplate(int roomIndex) RoomTemplate
  +getEnemyTemplatesForRoom(int roomIndex) std::vector~EnemyTemplate~
}

class BattleRoom {
  +BattleRoom(const RoomTemplate&, const std::vector~Hero~& party) void
  +isCleared() const bool
  +isPlayerWiped() const bool
  +getCoins() const int
  +update(float dt) void
  +render(sf::RenderWindow& window) void
}

class CombatEntity {
  +setTargetSelector(ITargetSelector*) void
}

class ITargetSelector {
  <<interface>>
  +selectTarget(const std::vector~CombatEntity*~& allies, const std::vector~CombatEntity*~& enemies) CombatEntity*
}

class ManualTargetSelector
class AutoTargetSelector

class PauseState
class TavernState
class Hero
class Item
class EnemyTemplate
class RoomTemplate

BattleState ..|> IGameState

BattleState o-- GameStateManager : uses to switch states
BattleState o-- GameWorld : reads/writes run data
BattleState *-- BattleRoom : creates current room

BattleState ..> PauseState : opens
BattleState ..> TavernState : returns after run

GameWorld --> AssetManager : room/enemy data
BattleRoom ..> RoomTemplate : uses
BattleRoom *-- CombatEntity : battle units
AssetManager --> RoomTemplate
AssetManager --> EnemyTemplate

CombatEntity o-- ITargetSelector : targeting strategy
ManualTargetSelector ..|> ITargetSelector
AutoTargetSelector ..|> ITargetSelector
BattleState ..> ITargetSelector : mode switch