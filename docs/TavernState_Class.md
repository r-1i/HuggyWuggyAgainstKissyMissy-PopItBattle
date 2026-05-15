---
config:
  layout: elk
  theme: redux
title: TavernState
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

class TavernState {
  -GameStateManager& gsm_
  -GameWorld& gameWorld_
  -std::vector~HeroRecruitCard~ availableCards_
  -Button btnGoToBattle_
  -Button btnUpgradeTavern_
  -Button btnPartyManagement_
  -Button btnExit_
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
  +getCoins() int
  +tryConsumeCoins(int amount) bool
  +recruitHero(const HeroTemplate&) bool
  +upgradeTavern() void
  +getAvailableHeroesForTavern() std::vector~HeroTemplate~
}

class HeroRecruitCard {
  -Button btnSelect_
  -HeroTemplate template_
  +render(sf::RenderWindow&) void
}

class Button {
  +std::function~void~ onClick
  +handleInput(sf::Event) void
  +render(sf::RenderWindow&) void
}

class RecruitModalState {

}

class PartyManagementState {
}

class HeroTemplate

TavernState ..|> IGameState

TavernState o-- GameStateManager : uses to switch states
TavernState o-- GameWorld : reads/writes game data

TavernState *-- Button : UI controls
TavernState *-- HeroRecruitCard : recruit list

TavernState ..> BattleState : starts battle
TavernState ..> PartyManagementState : opens
TavernState ..> RecruitModalState : opens
HeroRecruitCard --> HeroTemplate : holds data