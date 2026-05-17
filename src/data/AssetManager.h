#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../entities/EnemyTemplate.h"
#include "../entities/HeroTemplate.h"
#include "../entities/RoomTemplate.h"
#include "../items/ItemTemplate.h"
#include "../items/LootTable.h"
#include "IDataLoader.h"

class AssetManager {
 public:
  explicit AssetManager(std::unique_ptr<IDataLoader> loader)
      : loader_(std::move(loader)) {}

  void loadAllData() {
    heroes_ = loader_->loadHeroes(1);
    enemies_ = loader_->loadEnemies();
    rooms_ = loader_->loadRooms();
    items_ = loader_->loadItems();
  }

  std::vector<HeroTemplate> getAvailableHeroes(int tavernLevel) {
    return loader_->loadHeroes(tavernLevel);
  }

  std::vector<EnemyTemplate> getEnemyTemplatesForRoom(int roomIndex) {
    if (roomIndex < 0 || roomIndex >= static_cast<int>(rooms_.size()))
      return {};

    const auto& room = rooms_[roomIndex];
    std::vector<EnemyTemplate> result;
    for (const auto& enemy : enemies_) {
      for (const auto& type : room.possibleEnemyTypes) {
        if (enemy.type == type) {
          result.push_back(enemy);
          break;
        }
      }
    }
    return result;
  }

  RoomTemplate getRoomTemplate(int roomIndex) {
    if (roomIndex < 0 || roomIndex >= static_cast<int>(rooms_.size()))
      throw std::out_of_range("Room index out of range: " +
                              std::to_string(roomIndex));
    return rooms_[roomIndex];
  }

  ItemTemplate getItemTemplate(const std::string& id) {
    for (const auto& item : items_) {
      if (item.id == id) return item;
    }
    throw std::runtime_error("Item template not found: " + id);
  }

 private:
  std::unique_ptr<IDataLoader> loader_;
  std::vector<HeroTemplate> heroes_;
  std::vector<EnemyTemplate> enemies_;
  std::vector<RoomTemplate> rooms_;
  std::vector<ItemTemplate> items_;
};
