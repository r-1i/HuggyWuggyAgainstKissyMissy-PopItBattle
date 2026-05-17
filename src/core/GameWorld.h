#pragma once

#include <memory>
#include <vector>

#include "../entities/Hero.h"
#include "../entities/HeroTemplate.h"
#include "../items/Item.h"
#include "data/AssetManager.h"
#include "data/JsonDataLoader.h"

class GameWorld {
 private:
  AssetManager assetMgr_;
  std::vector<Hero> currentParty_;
  std::vector<std::unique_ptr<Item>> inventory_;
  int coins_;
  int tavernLevel_;

 public:
  const int kMaxSquadSize = 5;
  GameWorld();
  explicit GameWorld(std::unique_ptr<IDataLoader> loader);

  AssetManager& getAssetManager();
  int getCoins();
  std::vector<Hero>& getCurrentParty();

  void addCoins();
  bool tryConsumeCoins(int amount);
  void addItem(std::unique_ptr<Item> item);
  bool tryRecruitHero(const HeroTemplate& hero);
  void clearParty();
  void upgradeTavern();
  std::vector<HeroTemplate> getAvailibleHeroesForTavern();

  void startNewRun();
  void finishRun(int goldFromRun, std::vector<std::unique_ptr<Item>> loot);
};
