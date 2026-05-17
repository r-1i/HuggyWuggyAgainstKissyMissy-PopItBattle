#pragma once
#include <vector>
#include "LootEntry.h"

class LootTable {
public:
    LootTable() = default;
    explicit LootTable(std::vector<LootEntry> entries) : entries_(std::move(entries)) {}

    const std::vector<LootEntry>& getEntries() const { return entries_; }

private:
    std::vector<LootEntry> entries_;
};
