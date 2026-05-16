#pragma once
#include <vector>
#include <string>
#include "entities/HeroTemplate.h"
#include "entities/EnemyTemplate.h"
#include "entities/RoomTemplate.h"
#include "items/LootTable.h"
#include "items/ItemTemplate.h"

class IDataLoader {
public:
    virtual ~IDataLoader() = default;

    virtual std::vector<HeroTemplate>  loadHeroes(int tavernLevel) = 0;
    virtual std::vector<EnemyTemplate> loadEnemies() = 0;
    virtual std::vector<RoomTemplate>  loadRooms() = 0;
    virtual LootTable                  loadLootTable(std::string tableName) = 0;
    virtual std::vector<ItemTemplate>  loadItems() = 0;
};
