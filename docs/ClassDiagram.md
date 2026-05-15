---
config:
  layout: elk
  theme: dark
title: Huggy-Wuggy Against Kissy-Missy / Pop-It Battle
---
classDiagram
direction TB
    class Game {
        -GameStateManager gsm_
        -std::unique_ptr~GameWorld~ gameWorld_
        -AssetManager assetMgr_
    }


    class GameWorld {
        -AssetManager& assetMgr_
        -std::vector~Hero~ currentParty_
        -std::vector~std::unique_ptr~Item~~ inventory_
        -int coins_
        -int tavernLevel_

        +getAssetManager() AssetManager&
        +getCoins() int
        +addCoins(int amount) void
        +tryConsumeCoins(int amount) bool
        +addItem(std::unique_ptr~Item~ item) void
        +recruitHero(const HeroTemplate&) bool
        +startNewRun() void
        +finishRun(int goldFromRun, std::vector~std::unique_ptr~Item~~ loot) void
        +getCurrentParty() std::vector~Hero~&
        +clearParty() void
        +upgradeTavern() void
        +getAvailableHeroesForTavern() std::vector~HeroTemplate~
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
        -Button btnUpgradeTavern
        -Button btnExit
        -Button btnPartyManagement
    }

    class AssetManager {
        -std::unique_ptr~IDataLoader~ loader
        +AssetManager(std::unique_ptr~IDataLoader~ loader)
        +loadAllData() void
        +getAvailableHeroes(int tavernLevel) std::vector~HeroTemplate~
        +getEnemyTemplatesForRoom(int roomIndex) std::vector~EnemyTemplate~
        +getRoomTemplate(int roomIndex) RoomTemplate
        +getItemTemplate(std::string id) ItemTemplate
    }

    class EnemyTemplate {
        +std::string type
        +int hp
        +int damage
        +int coinsForDestroy
        +std::string spritePath
        +LootTable lootTable
    }

    class RoomTemplate {
        +std::string backgroundPath
        +std::string musicPath
        +int maxEnemies
        +std::vector~std::string~ possibleEnemyTypes
    }


    class Item {
        -std::string name
        -int price
        -std::unique_ptr~ItemEffect~ effect_
        +onEquip(Hero& hero) void
        +onUnequip(Hero& hero) void
        +onAttack(CombatEntity& attacker, CombatEntity& target) void
    }

    
    class ItemEffect{
        <<abstract>>
        virtual onEquip(Hero& hero) = 0 void
        virtual onUnequip(Hero& hero) = 0 void
        virtual onAttack(CombatEntity& attacker, CombatEntity& target) void
        virtual onTakeDamage(CombatEntity& owner, int& damageAmount) void
    }

    class AttackBonusEffect{
        -int bonusDamage_
        +onEquip(Hero& hero) override void
        +onUnequip(Hero& hero) override void
    }

    class DefenceBonusEffect{
        -int bonusDefence_
        +onEquip(Hero& hero) override void
        +onUnequip(Hero& hero) override void
    }


    class LootTable {
        -std::vector~LootEntry~ entries_
        +LootTable(const std::vector~LootEntry~ entries) void
        +roll(int count) std::vector~std::unique_ptr~Item~~
        +rollOne() std::unique_ptr~Item~
    }

    class LootEntry {
        +std::string itemId
        +int weight
        +int minQuantity
        +int maxQuantity
        +int price
    }

    class RecruitModalState {
        -GameStateManager& gsm_
        -HeroTemplate& heroTemplate_
        -sf::Rectangle background
        -Button btnClose_
        -Button btnBuy_
        -sf::Sprite heroSprite
        -std::string heroName
    }

    class BattleState {
        -GameStateManager& gsm_
        -GameWorld& gameWorld_
        -int coinsAddedThisState_
        -int currentRoomIndex_
        -const int TOTAL_ROOMS
        -BattleRoom currentRoom_
        -Button btnAutoHand
        -bool isManualMode
    }

    class PartyManagementState {
        -GameStateManager& gsm_
        -GameWorld& gameWorld_
        -std::vector~HeroEquipRow~ heroRows_
        +refresh() void
    }

    class HeroEquipRow {
        -Hero& hero
        -EquipSlot attackSlot
        -EquipSlot defenseSlot
        +render(sf::RenderWindow& window) void
    }

    class EquipSlot {
        -std::unique_ptr~Item~ equippedItem
        -sf::Sprite icon
        +handleInput(sf::Event) void
        +render(sf::RenderWindow&) void
        +onClick() void
    }

    class BattleRoom {
        +BattleRoom(const RoomTemplate&, const std::vector~Hero~& party) void
        -std::vector~std::unique_ptr~CombatEntity~~ enemies_
        -std::vector~std::unique_ptr~CombatEntity~~ playerParty_
        -RoomTemplate roomTemplate_
        -sf::Texture backgroundTexture
        -sf::Music roomMusic
        +isCleared() const bool
        +isPlayerWiped() const bool
        +getCoins() const int
        +update(float dt) void
        +render(sf::RenderWindow& window) void
    }

    class PauseState {
        -GameStateManager& gsm_
        -GameWorld& gameWorld_
    }

    class CombatEntity {
        +CombatEntity(const Hero&) void
        +CombatEntity(const EnemyTemplate&) void
        -int hp
        -int maxHp
        -int damage
        -int bonusDamage
        -int bonusArmor
        -ITargetSelector* targetSelector
        -sf::Texture texture
        -sf::SoundBuffer deathSound
        -int coinsForDestroy
        -LootTable lootTable
        +update(float dt) void
        +attack(CombatEntity& target) void
        +isAlive() const bool
        +setTargetSelector(ITargetSelector*) void
        +CombatEntity* chooseTarget(const std::vector~CombatEntity*~& allies, const std::vector~CombatEntity*~& enemies)

    }

    class Hero {
        -std::unique_ptr~Item~ equipedArmor
        -std::unique_ptr~Item~ equipedWeapon
        +std::string name
        +int price
        +int maxHealth
        +int damage
        +void equipWeapon(std::unique_ptr~Item~)
        +void equipArmor(std::unique_ptr~Item~)
    }

    class ITargetSelector {
        <<interface>>
        +selectTarget(const std::vector~CombatEntity*~& allies, const std::vector~CombatEntity*~& enemies) CombatEntity*
    }

    class ManualTargetSelector {
    }

    class AutoTargetSelector {
    }

    class HeroTemplate {
        +int maxHp
        +int damage
        +int price
    }

    class ItemTemplate {
        +std::string name
        +int value
        +int price
    }

    class Button {
        -sf::Rectangle shape
        -sf::Text text
        +std::function~void~ onClick
        +handleInput(sf::Event) void
        +render(sf::RenderWindow&) void
    }

    class HeroRecruitCard {
        -Button btnSelect_
        -HeroTemplate template_
        -sf::Sprite sprite_
        +render(sf::RenderWindow&) void
    }

    class IDataLoader {
    <<interface>>
        +virtual std::vector~HeroTemplate~ loadHeroes(int tavernLevel) = 0
        +virtual std::vector~EnemyTemplate~ loadEnemies() = 0
        +virtual std::vector~RoomTemplate~ loadRooms() = 0
        +virtual LootTable loadLootTable(std::string tableName) = 0
        +virtual std::vector~ItemTemplate~ loadItems() = 0
    }

    class JsonDataLoader {
        +JsonDataLoader(std::string dataFolderPath)
        +std::vector~HeroTemplate~ loadHeroes(int tavernLevel) override
        +std::vector~EnemyTemplate~ loadEnemies() override
        +std::vector~RoomTemplate~ loadRooms() override
        +LootTable loadLootTable(std::string tableName) override
        +std::vector~ItemTemplate~ loadItems() override
    }


    Game *-- GameStateManager
    Game *-- GameWorld
    GameStateManager *-- IGameState
    GameStateManager o-- GameWorld
    TavernState ..|> IGameState
    BattleState ..|> IGameState
    PauseState ..|> IGameState
    RecruitModalState ..|> IGameState
    BattleState *-- BattleRoom
    BattleRoom *-- CombatEntity
    Hero --> CombatEntity
    Hero --* GameWorld
    Game *-- AssetManager
    TavernState *-- HeroRecruitCard
    TavernState *-- Button
    HeroRecruitCard --> HeroTemplate
    HeroRecruitCard *-- Button
    HeroTemplate ..> Hero : used to create
    RecruitModalState --> HeroTemplate
    EnemyTemplate ..> CombatEntity
    EnemyTemplate *-- LootTable
    TavernState ..> RecruitModalState : opens
    TavernState ..> PartyManagementState : opens
    PartyManagementState *-- HeroEquipRow
    HeroEquipRow *-- EquipSlot
    EquipSlot *-- Item
    PartyManagementState ..|> IGameState
    BattleState *-- Button
    CombatEntity o-- ITargetSelector : uses
    ManualTargetSelector ..|> ITargetSelector
    AutoTargetSelector ..|> ITargetSelector
    BattleState --> ITargetSelector
    LootTable ..> Item
    Hero o-- Item
    BattleRoom o-- RoomTemplate
    GameWorld --> AssetManager : uses
    AssetManager --> HeroTemplate
    AssetManager --> EnemyTemplate
    AssetManager --> RoomTemplate
    AssetManager --> LootTable
    RecruitModalState --> GameWorld
    CombatEntity o-- LootTable
    HeroEquipRow --> Hero
    AssetManager *-- IDataLoader
    JsonDataLoader ..|> IDataLoader
    JsonDataLoader --> HeroTemplate
    JsonDataLoader --> EnemyTemplate
    JsonDataLoader --> RoomTemplate
    JsonDataLoader --> LootTable
    JsonDataLoader --> ItemTemplate
    AssetManager --> ItemTemplate
    Item *-- ItemEffect  
    AttackBonusEffect ..|> ItemEffect
    DefenceBonusEffect ..|> ItemEffect
    