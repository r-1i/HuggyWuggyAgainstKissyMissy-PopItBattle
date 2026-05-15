---
config:
  layout: elk
  theme: dark
title: High Level Architecture (Game / States)
---

## Архитектура верхнего уровня

Ниже — упрощённая UML (Mermaid) диаграмма верхнего уровня. Цель: **явно показать, что `GameStateManager` управляет переключением состояний** (стек `IGameState`, `pushState()`/`popState()`), а `Game` владеет основными подсистемами.

```mermaid
classDiagram
direction TB

class Game {
  -GameStateManager gsm_
  -std::unique_ptr~GameWorld~ gameWorld_
  -AssetManager assetMgr_
}

class GameStateManager {
  -std::stack~std::unique_ptr~IGameState~~ states_
  -GameWorld& gameWorld_
  +pushState(std::unique_ptr~IGameState~ state) void
  +popState() void
  +handleInput(sf::Event event) void
  +update(float dt) void
  +render(sf::RenderWindow& window) void
}

class IGameState {
  <<interface>>
  +handleInput(sf::Event event) = 0 void
  +update(float dt) = 0 void
  +onResume() = 0 void
  +render(sf::RenderWindow& window) = 0 void
  +shouldRenderBelow() = 0 const bool
  +shouldUpdateBelow() = 0 const bool
}

class GameWorld {
  -AssetManager& assetMgr_
}

class AssetManager {
  -std::unique_ptr~IDataLoader~ loader
  +loadAllData() void
}

class IDataLoader {
  <<interface>>
  +loadHeroes(int tavernLevel) std::vector~HeroTemplate~
  +loadEnemies() std::vector~EnemyTemplate~
  +loadRooms() std::vector~RoomTemplate~
  +loadLootTable(std::string tableName) LootTable
  +loadItems() std::vector~ItemTemplate~
}

class JsonDataLoader {
  +JsonDataLoader(std::string dataFolderPath)
}

class TavernState
class BattleState
class PauseState
class RecruitModalState
class PartyManagementState

Game *-- GameStateManager
Game *-- GameWorld
Game *-- AssetManager

GameStateManager o-- GameWorld : uses
GameStateManager *-- IGameState : manages stack

TavernState ..|> IGameState
BattleState ..|> IGameState
PauseState ..|> IGameState
RecruitModalState ..|> IGameState
PartyManagementState ..|> IGameState

GameWorld --> AssetManager : uses
AssetManager *-- IDataLoader : loader
JsonDataLoader ..|> IDataLoader
```

## Как читать диаграмму

- **Переключение состояний**: `GameStateManager` **владеет стеком** `states_ : stack<unique_ptr<IGameState>>` и делает `pushState()`/`popState()`. Активное состояние — вершина стека.
- **Жизненный цикл**: `GameStateManager` проксирует события/кадры в текущее состояние через `handleInput()`/`update()`/`render()`.
- **Владение данными**: `Game` владеет `GameWorld` и `AssetManager`, а `GameWorld` использует `AssetManager` по ссылке (`AssetManager&`).
- **Загрузка данных**: `AssetManager` работает через абстракцию `IDataLoader`; конкретная реализация — `JsonDataLoader`.

