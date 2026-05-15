#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "assets/AssetManager.h"
#include "core/GameStateManager.h"
#include "core/GameWorld.h"

class Game {
 private:
  GameStateManager gsm_;
  AssetManager assetMgr_;
  GameWorld gameWorld_;
  sf::RenderWindow renderWindow_;

 public:
  Game();
  void run();
};
