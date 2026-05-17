#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameStateManager.h"
#include "GameWorld.h"
#include "data/AssetManager.h"

class Game {
 private:
  GameWorld gameWorld_;
  GameStateManager gsm_;
  sf::RenderWindow renderWindow_;

 public:
  Game();
  void run();
};
