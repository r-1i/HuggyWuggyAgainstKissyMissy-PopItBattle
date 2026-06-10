#pragma once
#include <random>
#include <string>
#include <vector>
#include "LootEntry.h"

class LootTable {
public:
    int itemCountMin = 0;
    int itemCountMax = 0;
    int coinsMin = 0;
    int coinsMax = 0;
    std::vector<LootEntry> items;

    int rollCoins(std::mt19937& rng) const;
    std::vector<std::string> rollItemIds(std::mt19937& rng) const;
};
