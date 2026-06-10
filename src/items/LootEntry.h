#pragma once
#include <string>

struct LootEntry {
    std::string itemId;
    int weight = 1;
    int minQuantity = 1;
    int maxQuantity = 1;
};
