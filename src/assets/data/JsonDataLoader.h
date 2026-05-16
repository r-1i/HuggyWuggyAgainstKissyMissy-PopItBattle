#pragma once
#include "IDataLoader.h"
#include <string>

class JsonDataLoader : public IDataLoader {
public:
    explicit JsonDataLoader(std::string dataFolderPath);

    std::vector<HeroTemplate>  loadHeroes(int tavernLevel) override;
    std::vector<EnemyTemplate> loadEnemies() override;
    std::vector<RoomTemplate>  loadRooms() override;
    LootTable                  loadLootTable(std::string tableName) override;
    std::vector<ItemTemplate>  loadItems() override;

private:
    std::string dataFolderPath_;
};
