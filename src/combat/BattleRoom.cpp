#include "BattleRoom.h"

#include "../data/AssetManager.h"
#include "../items/ItemFactory.h"

BattleRoom::BattleRoom(const BattleConfigEntry& config, RoomKind kind,
                       std::vector<Hero>& party, AssetManager& assetMgr)
    : assetMgr_(assetMgr),
      rng_(std::random_device{}()),
      bgSprite_(bgTexture_),
      font_("assets/fonts/DelaGothicOne-Regular.ttf") {
  bgTexture_.loadFromFile(config.backgroundPath);
  bgSprite_.setTexture(bgTexture_, true);
  sf::Vector2u bgSize = bgTexture_.getSize();
  if (bgSize.x > 0 && bgSize.y > 0)
    bgSprite_.setScale({1280.f / bgSize.x, 720.f / bgSize.y});
  bgSprite_.setPosition({0.f, 0.f});

  spawnPlayers(party);
  spawnEnemies(config, kind, assetMgr);
}

void BattleRoom::spawnPlayers(std::vector<Hero>& party) {
  int index = 0;
  for (auto& hero : party) {
    if (hero.currentHealth <= 0) continue;
    playerEntities_.push_back(
        std::make_unique<CombatEntity>(hero, formationPosition(index, true)));
    index++;
  }
}

void BattleRoom::spawnEnemies(const BattleConfigEntry& config, RoomKind kind,
                              AssetManager& assetMgr) {
  std::vector<const EnemyTemplate*> templates;

  if (kind == RoomKind::Chest) {
    templates.push_back(&assetMgr.getEnemyTemplate(config.chestPath));
  } else if (kind == RoomKind::Boss) {
    templates.push_back(&assetMgr.getEnemyTemplate(config.bossPath));
  } else if (!config.enemyGroups.empty()) {
    int totalWeight = 0;
    for (const auto& group : config.enemyGroups) totalWeight += group.weight;

    std::uniform_int_distribution<int> countDist(config.minEnemies, config.maxEnemies);
    int count = countDist(rng_);

    for (int i = 0; i < count; i++) {
      std::uniform_int_distribution<int> weightDist(1, totalWeight);
      int roll = weightDist(rng_);
      for (const auto& group : config.enemyGroups) {
        roll -= group.weight;
        if (roll <= 0) {
          templates.push_back(&assetMgr.getEnemyTemplate(group.path));
          break;
        }
      }
    }
  }

  for (size_t i = 0; i < templates.size(); i++) {
    enemyEntities_.push_back(std::make_unique<CombatEntity>(
        *templates[i], formationPosition(static_cast<int>(i), false)));
  }
}

sf::Vector2f BattleRoom::formationPosition(int index, bool isPlayerSide) const {
  float baseX = isPlayerSide ? 150.f : 880.f;
  float column = static_cast<float>(index % 2);
  float row = static_cast<float>(index / 2);
  return {baseX + column * 90.f, 140.f + row * 160.f + column * 80.f};
}

void BattleRoom::update(float dt) {
  if (phase_ == Phase::Done) return;

  ensureCurrentTarget();

  if (phase_ == Phase::PlayerAttack && isManualMode_ && !isCleared() && !hasValidManualTarget())
    return;

  attackTimer_ += dt;
  if (attackTimer_ < kAttackDelay) return;
  attackTimer_ = 0.f;

  advanceTurn();
}

CombatEntity* BattleRoom::getCurrentActor() const {
  if (phase_ == Phase::Done) return nullptr;

  const auto& actors = (phase_ == Phase::PlayerAttack) ? playerEntities_ : enemyEntities_;
  for (size_t i = actorIndex_; i < actors.size(); i++)
    if (actors[i]->isAlive()) return actors[i].get();

  return nullptr;
}

void BattleRoom::ensureCurrentTarget() {
  CombatEntity* actor = getCurrentActor();
  if (!actor) {
    currentTarget_ = nullptr;
    return;
  }

  if (phase_ == Phase::PlayerAttack && isManualMode_) {
    currentTarget_ = hasValidManualTarget() ? manualSelector_.getSelected() : nullptr;
    return;
  }

  const auto& targetSide = (phase_ == Phase::PlayerAttack) ? enemyEntities_ : playerEntities_;

  if (currentTarget_ && currentTarget_->isAlive()) {
    for (const auto& target : targetSide)
      if (target.get() == currentTarget_) return;
  }

  std::vector<CombatEntity*> targets;
  for (const auto& target : targetSide) targets.push_back(target.get());
  currentTarget_ = autoSelector_.selectTarget({}, targets);
}

bool BattleRoom::hasValidManualTarget() const {
  CombatEntity* selected = manualSelector_.getSelected();
  if (!selected || !selected->isAlive()) return false;

  for (const auto& enemy : enemyEntities_)
    if (enemy.get() == selected) return true;

  return false;
}

void BattleRoom::advanceTurn() {
  if (phase_ == Phase::PlayerAttack) {
    while (actorIndex_ < playerEntities_.size() && !playerEntities_[actorIndex_]->isAlive())
      actorIndex_++;

    if (actorIndex_ < playerEntities_.size()) {
      performAttack(playerEntities_[actorIndex_].get());
      actorIndex_++;
    }

    if (actorIndex_ >= playerEntities_.size()) {
      if (isCleared()) {
        phase_ = Phase::Done;
        return;
      }
      phase_ = Phase::EnemyAttack;
      actorIndex_ = 0;
    }
  } else if (phase_ == Phase::EnemyAttack) {
    while (actorIndex_ < enemyEntities_.size() && !enemyEntities_[actorIndex_]->isAlive())
      actorIndex_++;

    if (actorIndex_ < enemyEntities_.size()) {
      performAttack(enemyEntities_[actorIndex_].get());
      actorIndex_++;
    }

    if (actorIndex_ >= enemyEntities_.size()) {
      if (isPlayerWiped()) {
        phase_ = Phase::Done;
        return;
      }
      phase_ = Phase::PlayerAttack;
      actorIndex_ = 0;
    }
  }
}

void BattleRoom::performAttack(CombatEntity* attacker) {
  if (!attacker->isAlive()) return;

  CombatEntity* target = currentTarget_;
  if (!target || !target->isAlive()) return;

  attacker->attack(*target);

  if (!target->isAlive() && !target->isPlayerSide()) onEnemyKilled(*target);

  currentTarget_ = nullptr;

  if (phase_ == Phase::PlayerAttack && isManualMode_) manualSelector_.setSelected(nullptr);
}

void BattleRoom::onEnemyKilled(CombatEntity& enemy) {
  const LootTable* loot = enemy.getLootTable();
  if (!loot) return;

  coinsEarned_ += loot->rollCoins(rng_);

  for (const auto& itemId : loot->rollItemIds(rng_))
    lootEarned_.push_back(makeItem(assetMgr_.getItemTemplate(itemId)));
}

bool BattleRoom::isCleared() const {
  for (const auto& enemy : enemyEntities_)
    if (enemy->isAlive()) return false;
  return true;
}

bool BattleRoom::isPlayerWiped() const {
  for (const auto& player : playerEntities_)
    if (player->isAlive()) return false;
  return true;
}

std::vector<std::unique_ptr<Item>> BattleRoom::takeLootEarned() {
  return std::move(lootEarned_);
}

void BattleRoom::handleClick(sf::Vector2f position) {
  if (!isManualMode_) return;

  for (auto& enemy : enemyEntities_) {
    if (enemy->isAlive() && enemy->getSprite().getGlobalBounds().contains(position)) {
      manualSelector_.setSelected(enemy.get());
      return;
    }
  }
}

void BattleRoom::render(sf::RenderWindow& window) {
  window.draw(bgSprite_);

  for (auto& player : playerEntities_) renderEntity(window, *player);
  for (auto& enemy : enemyEntities_) renderEntity(window, *enemy);
}

void BattleRoom::renderEntity(sf::RenderWindow& window, CombatEntity& entity) {
  if (!entity.isAlive()) return;

  window.draw(entity.getSprite());

  sf::FloatRect spriteBounds = entity.getSprite().getGlobalBounds();

  std::string hpString = std::to_string(entity.getCurrentHp()) + " HP";
  sf::Text hpText(font_, hpString, 18U);
  sf::FloatRect textBounds = hpText.getLocalBounds();

  sf::RectangleShape labelBg;
  labelBg.setSize({textBounds.size.x + 12.f, textBounds.size.y + 10.f});
  labelBg.setFillColor(sf::Color(0, 0, 0, 160));
  labelBg.setPosition({spriteBounds.position.x + spriteBounds.size.x / 2.f - labelBg.getSize().x / 2.f,
                        spriteBounds.position.y - labelBg.getSize().y - 6.f});

  hpText.setFillColor(sf::Color::White);
  hpText.setPosition({labelBg.getPosition().x + 6.f, labelBg.getPosition().y + 3.f});

  window.draw(labelBg);
  window.draw(hpText);

  if (currentTarget_ == &entity) {
    sf::CircleShape indicator(8.f, 3);
    indicator.setFillColor(sf::Color::Yellow);
    indicator.setPosition({spriteBounds.position.x + spriteBounds.size.x / 2.f - 8.f,
                            labelBg.getPosition().y - 20.f});
    window.draw(indicator);
  }

  if (getCurrentActor() == &entity) {
    sf::RectangleShape turnIndicator;
    turnIndicator.setSize({spriteBounds.size.x, 4.f});
    turnIndicator.setFillColor(sf::Color::Yellow);
    turnIndicator.setPosition({spriteBounds.position.x, spriteBounds.position.y + spriteBounds.size.y});
    window.draw(turnIndicator);
  }
}
