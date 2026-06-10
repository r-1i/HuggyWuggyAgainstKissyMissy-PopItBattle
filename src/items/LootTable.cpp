#include "LootTable.h"

int LootTable::rollCoins(std::mt19937& rng) const {
    if (coinsMax <= 0) return 0;
    std::uniform_int_distribution<int> dist(coinsMin, coinsMax);
    return dist(rng);
}

std::vector<std::string> LootTable::rollItemIds(std::mt19937& rng) const {
    std::vector<std::string> result;
    if (itemCountMax <= 0 || items.empty()) return result;

    std::uniform_int_distribution<int> countDist(itemCountMin, itemCountMax);
    int count = countDist(rng);

    int totalWeight = 0;
    for (const auto& entry : items) totalWeight += entry.weight;
    if (totalWeight <= 0) return result;

    std::uniform_int_distribution<int> weightDist(1, totalWeight);

    for (int i = 0; i < count; i++) {
        int roll = weightDist(rng);
        for (const auto& entry : items) {
            roll -= entry.weight;
            if (roll <= 0) {
                std::uniform_int_distribution<int> qtyDist(entry.minQuantity, entry.maxQuantity);
                int qty = qtyDist(rng);
                for (int q = 0; q < qty; q++) result.push_back(entry.itemId);
                break;
            }
        }
    }

    return result;
}
