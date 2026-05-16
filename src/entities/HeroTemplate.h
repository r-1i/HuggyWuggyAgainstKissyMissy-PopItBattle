#pragma once
#include <string>

struct HeroTemplate {
    std::string name;
    int maxHp = 0;
    int damage = 0;
    int price = 0;
    int minTavernLevel = 1;
    std::string spritePath;
};
