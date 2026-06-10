#pragma once
#include <string>

struct ItemTemplate {
    std::string id;
    std::string name;
    std::string type;       // "weapon" or "armor"
    int effectValue = 0;
    int value = 0;
    int price = 0;
    std::string spritePath;
};
