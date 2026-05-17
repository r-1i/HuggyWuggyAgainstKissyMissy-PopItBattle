#pragma once
#include <string>

struct HeroTemplate {
 public:
  HeroTemplate() = default;
  HeroTemplate(const std::string& name, int maxHp, int damage, int price)
      : name(name), maxHp(maxHp), damage(damage), price(price) {};
  std::string name;
  int maxHp = 0;
  int damage = 0;
  int price = 0;
  int minTavernLevel = 0;
  std::string spritePath;
};
