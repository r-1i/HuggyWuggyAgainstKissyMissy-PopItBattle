#include "GameWorld.h"

GameWorld::GameWorld() : coins_(0), tavernLevel_(0) {}

AssetManager& GameWorld::getAssetManager() { return assetMgr_; }

int GameWorld::getCoins() { return coins_; }

std::vector<Hero>& GameWorld::getCurrentParty() { return currentParty_; }

void GameWorld::addCoins() {}

bool GameWorld::tryConsumeCoins(int amount) { return false; }

void GameWorld::addItem(std::unique_ptr<Item> item) {}

bool GameWorld::tryRecruitHero(const HeroTemplate& hero) { return false; }

void GameWorld::clearParty() {}

void GameWorld::upgradeTavern() {}

std::vector<HeroTemplate> GameWorld::getAvailibleHeroesForTavern() {
  return std::vector<HeroTemplate>();
}

void GameWorld::startNewRun() {}

void GameWorld::finishRun(int goldFromRun,
                          std::vector<std::unique_ptr<Item>> loot) {}
