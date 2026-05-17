#pragma once
#include <string>
#include "../items/LootTable.h"

struct EnemyTemplate {
    std::string type;
    int hp = 0;
    int damage = 0;
    int coinsForDestroy = 0;
    std::string spritePath;
    LootTable lootTable;
};
