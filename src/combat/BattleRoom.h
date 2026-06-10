#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

#include "AutoTargetSelector.h"
#include "BattleConfig.h"
#include "CombatEntity.h"
#include "ManualTargetSelector.h"
#include "../entities/Hero.h"
#include "../items/Item.h"

class AssetManager;

class BattleRoom {
 public:
  BattleRoom(const BattleConfigEntry& config, RoomKind kind,
             std::vector<Hero>& party, AssetManager& assetMgr);

  void update(float dt);
  void render(sf::RenderWindow& window);
  void handleClick(sf::Vector2f position);

  void setManualMode(bool manual) { isManualMode_ = manual; }

  bool isCleared() const;
  bool isPlayerWiped() const;
  bool isFinished() const { return phase_ == Phase::Done; }

  int getCoinsEarned() const { return coinsEarned_; }
  std::vector<std::unique_ptr<Item>> takeLootEarned();

 private:
  enum class Phase { PlayerAttack, EnemyAttack, Done };

  void spawnPlayers(std::vector<Hero>& party);
  void spawnEnemies(const BattleConfigEntry& config, RoomKind kind, AssetManager& assetMgr);
  sf::Vector2f formationPosition(int index, bool isPlayerSide) const;

  void advanceTurn();
  void performAttack(CombatEntity* attacker);
  void onEnemyKilled(CombatEntity& enemy);
  bool hasValidManualTarget() const;
  CombatEntity* getCurrentActor() const;
  void ensureCurrentTarget();

  void renderEntity(sf::RenderWindow& window, CombatEntity& entity);

  AssetManager& assetMgr_;

  std::vector<std::unique_ptr<CombatEntity>> playerEntities_;
  std::vector<std::unique_ptr<CombatEntity>> enemyEntities_;

  AutoTargetSelector autoSelector_;
  ManualTargetSelector manualSelector_;
  bool isManualMode_ = false;

  Phase phase_ = Phase::PlayerAttack;
  size_t actorIndex_ = 0;
  CombatEntity* currentTarget_ = nullptr;
  float attackTimer_ = 0.f;
  static constexpr float kAttackDelay = 1.f;

  int coinsEarned_ = 0;
  std::vector<std::unique_ptr<Item>> lootEarned_;

  std::mt19937 rng_;

  sf::Texture bgTexture_;
  sf::Sprite bgSprite_;
  sf::Font font_;
};
