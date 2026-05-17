#pragma once

#include <string>
#include <vector>

#include "../entities/EnemyTemplate.h"
#include "../entities/HeroTemplate.h"
#include "../entities/RoomTemplate.h"
#include "../items/ItemTemplate.h"
#include "../items/LootTable.h"

class IDataLoader {
 public:
  virtual std::vector<HeroTemplate> loadHeroes(int tavernLevel) = 0;
  virtual std::vector<EnemyTemplate> loadEnemies() = 0;
  virtual std::vector<RoomTemplate> loadRooms() = 0;
  virtual LootTable loadLootTable(const std::string& tableName) = 0;
  virtual std::vector<ItemTemplate> loadItems() = 0;

  virtual ~IDataLoader() = default;
};
