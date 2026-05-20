#pragma once

#include <entities/HeroTemplate.h>
#include <items/Item.h>

#include <memory>
#include <string>

class Hero {
 private:
  std::unique_ptr<Item> equipedArmor;
  std::unique_ptr<Item> equipedWeapon;

 public:
  Hero(const HeroTemplate& heroTemplate);
  std::string name;
  int price;
  int maxHealth;
  int damage;

  void equipWeapon(std::unique_ptr<Item>);
  void equipArmor(std::unique_ptr<Item>);
};
