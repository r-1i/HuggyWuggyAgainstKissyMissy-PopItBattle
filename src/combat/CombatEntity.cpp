#include "CombatEntity.h"

#include <cmath>

#include "../items/Item.h"

constexpr float kEntitySize = 120.f;

CombatEntity::CombatEntity(Hero& hero, sf::Vector2f position)
    : name_(hero.name),
      maxHp_(hero.maxHealth),
      currentHp_(hero.currentHealth),
      damage_(hero.damage),
      armor_(hero.armor),
      isPlayer_(true),
      hero_(&hero),
      sprite_(texture_) {
  setupSprite(hero.spritePath, position);
}

CombatEntity::CombatEntity(const EnemyTemplate& enemyTemplate, sf::Vector2f position)
    : name_(enemyTemplate.name),
      maxHp_(enemyTemplate.hp),
      currentHp_(enemyTemplate.hp),
      damage_(enemyTemplate.damage),
      armor_(0),
      isPlayer_(false),
      hero_(nullptr),
      lootTable_(&enemyTemplate.lootTable),
      sprite_(texture_) {
  setupSprite(enemyTemplate.spritePath, position);
}

void CombatEntity::setupSprite(const std::string& path, sf::Vector2f position) {
  texture_.loadFromFile(path);
  sprite_.setTexture(texture_, true);
  sf::Vector2u size = texture_.getSize();
  if (size.x > 0 && size.y > 0)
    sprite_.setScale({kEntitySize / size.x, kEntitySize / size.y});
  sprite_.setPosition(position);
}

void CombatEntity::attack(CombatEntity& target) {
  if (!isAlive() || !target.isAlive()) return;

  if (hero_) {
    if (Item* weapon = hero_->getWeapon()) weapon->onAttack(*this, target);
  }

  target.takeDamage(damage_);
}

void CombatEntity::takeDamage(int amount) {
  int reduction = static_cast<int>(std::ceil(amount * armor_ / 100.0));
  int dmg = amount - reduction;
  if (dmg < 0) dmg = 0;

  if (hero_) {
    if (Item* armorItem = hero_->getArmor()) armorItem->onTakeDamage(*this, dmg);
  }

  currentHp_ -= dmg;
  if (currentHp_ < 0) currentHp_ = 0;

  if (hero_) hero_->currentHealth = currentHp_;
}
