#include "GameWorld.h"

#include "data/JsonDataLoader.h"

GameWorld::GameWorld()
    : assetMgr_(std::make_unique<JsonDataLoader>("assets/data")),
      coins_(100),
      tavernLevel_(1) {}

GameWorld::GameWorld(std::unique_ptr<IDataLoader> loader)
    : assetMgr_(std::move(loader)), coins_(100), tavernLevel_(1) {}

AssetManager& GameWorld::getAssetManager() { return assetMgr_; }

int GameWorld::getCoins() { return coins_; }

std::vector<Hero>& GameWorld::getCurrentParty() { return currentParty_; }

void GameWorld::addCoins() {}

bool GameWorld::tryConsumeCoins(int amount) {
  if (coins_ >= amount) {
    coins_ -= amount;
    return true;
  }
  return false;
}

void GameWorld::addItem(std::unique_ptr<Item> item) {}

bool GameWorld::tryRecruitHero(const HeroTemplate& heroTemplate) {
  if (currentParty_.size() < kMaxSquadSize) {
    currentParty_.push_back(Hero(heroTemplate));
    return true;
  }
  return false;
}

void GameWorld::clearParty() {}

void GameWorld::upgradeTavern() {}

std::vector<HeroTemplate> GameWorld::getAvailibleHeroesForTavern() {
  return assetMgr_.getAvailableHeroes(tavernLevel_);
}

void GameWorld::startNewRun() {}

void GameWorld::finishRun(int goldFromRun,
                          std::vector<std::unique_ptr<Item>> loot) {}
