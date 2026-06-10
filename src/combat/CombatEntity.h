#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../entities/EnemyTemplate.h"
#include "../entities/Hero.h"
#include "../items/LootTable.h"

class ITargetSelector;

class CombatEntity {
 public:
  CombatEntity(Hero& hero, sf::Vector2f position);
  CombatEntity(const EnemyTemplate& enemyTemplate, sf::Vector2f position);

  bool isAlive() const { return currentHp_ > 0; }
  int getCurrentHp() const { return currentHp_; }
  int getMaxHp() const { return maxHp_; }
  int getDamage() const { return damage_; }
  const std::string& getName() const { return name_; }
  bool isPlayerSide() const { return isPlayer_; }
  Hero* getHero() const { return hero_; }
  const LootTable* getLootTable() const { return lootTable_; }

  sf::Sprite& getSprite() { return sprite_; }
  const sf::Sprite& getSprite() const { return sprite_; }

  void attack(CombatEntity& target);
  void takeDamage(int amount);

  void setTargetSelector(ITargetSelector* selector) { targetSelector_ = selector; }
  ITargetSelector* getTargetSelector() const { return targetSelector_; }

 private:
  void setupSprite(const std::string& path, sf::Vector2f position);

  std::string name_;
  int maxHp_;
  int currentHp_;
  int damage_;
  int armor_;
  bool isPlayer_;
  Hero* hero_ = nullptr;
  const LootTable* lootTable_ = nullptr;
  sf::Texture texture_;
  sf::Sprite sprite_;
  ITargetSelector* targetSelector_ = nullptr;
};
