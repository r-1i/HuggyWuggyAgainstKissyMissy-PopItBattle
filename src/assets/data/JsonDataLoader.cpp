#include "JsonDataLoader.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static json loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open data file: " + path);
    return json::parse(file);
}

static LootTable parseLootTable(const json& arr) {
    std::vector<LootEntry> entries;
    for (const auto& e : arr) {
        LootEntry entry;
        entry.itemId      = e.at("itemId").get<std::string>();
        entry.weight      = e.value("weight",      1);
        entry.minQuantity = e.value("minQuantity",  1);
        entry.maxQuantity = e.value("maxQuantity",  1);
        entry.price       = e.value("price",        0);
        entries.push_back(std::move(entry));
    }
    return LootTable(std::move(entries));
}

// ---------------------------------------------------------------------------
// public
// ---------------------------------------------------------------------------

JsonDataLoader::JsonDataLoader(std::string dataFolderPath)
    : dataFolderPath_(std::move(dataFolderPath)) {}

std::vector<HeroTemplate> JsonDataLoader::loadHeroes(int tavernLevel) {
    auto j = loadFile(dataFolderPath_ + "/heroes.json");

    std::vector<HeroTemplate> result;
    for (const auto& item : j) {
        if (item.value("minTavernLevel", 1) > tavernLevel)
            continue;

        HeroTemplate h;
        h.name            = item.at("name").get<std::string>();
        h.maxHp           = item.at("maxHp").get<int>();
        h.damage          = item.at("damage").get<int>();
        h.price           = item.at("price").get<int>();
        h.minTavernLevel  = item.value("minTavernLevel", 1);
        h.spritePath      = item.value("spritePath", "");
        result.push_back(std::move(h));
    }
    return result;
}

std::vector<EnemyTemplate> JsonDataLoader::loadEnemies() {
    auto j = loadFile(dataFolderPath_ + "/enemies.json");

    std::vector<EnemyTemplate> result;
    for (const auto& item : j) {
        EnemyTemplate e;
        e.type            = item.at("type").get<std::string>();
        e.hp              = item.at("hp").get<int>();
        e.damage          = item.at("damage").get<int>();
        e.coinsForDestroy = item.value("coinsForDestroy", 0);
        e.spritePath      = item.value("spritePath", "");

        if (item.contains("lootTable"))
            e.lootTable = parseLootTable(item.at("lootTable"));

        result.push_back(std::move(e));
    }
    return result;
}

std::vector<RoomTemplate> JsonDataLoader::loadRooms() {
    auto j = loadFile(dataFolderPath_ + "/rooms.json");

    std::vector<RoomTemplate> result;
    for (const auto& item : j) {
        RoomTemplate r;
        r.backgroundPath     = item.value("backgroundPath", "");
        r.musicPath          = item.value("musicPath", "");
        r.maxEnemies         = item.at("maxEnemies").get<int>();
        r.possibleEnemyTypes = item.at("possibleEnemyTypes").get<std::vector<std::string>>();
        result.push_back(std::move(r));
    }
    return result;
}

LootTable JsonDataLoader::loadLootTable(std::string tableName) {
    auto j = loadFile(dataFolderPath_ + "/loot_tables.json");

    if (!j.contains(tableName))
        throw std::runtime_error("Loot table not found: " + tableName);

    return parseLootTable(j.at(tableName));
}

std::vector<ItemTemplate> JsonDataLoader::loadItems() {
    auto j = loadFile(dataFolderPath_ + "/items.json");

    std::vector<ItemTemplate> result;
    for (const auto& item : j) {
        ItemTemplate t;
        t.id    = item.at("id").get<std::string>();
        t.name  = item.at("name").get<std::string>();
        t.value = item.value("value", 0);
        t.price = item.value("price", 0);
        result.push_back(std::move(t));
    }
    return result;
}
