#pragma once
#include <string>
#include <vector>

struct RoomTemplate {
    std::string backgroundPath;
    std::string musicPath;
    int maxEnemies = 0;
    std::vector<std::string> possibleEnemyTypes;
};
