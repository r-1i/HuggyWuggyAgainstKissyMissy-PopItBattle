#pragma once
#include <string>
#include <vector>

enum class RoomKind { Normal, Chest, Boss };

struct EnemyGroupConfig {
    std::string path;
    int weight = 1;
};

struct BattleConfigEntry {
    std::string backgroundPath;
    std::vector<EnemyGroupConfig> enemyGroups;
    int minEnemies = 1;
    int maxEnemies = 1;
    std::string chestPath;
    std::string bossPath;
};
