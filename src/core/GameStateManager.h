#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

#include "core/GameWorld.h"
#include "states/IGameState.h"

class GameStateManager {
 private:
  std::vector<std::unique_ptr<IGameState>> states_;
  GameWorld& gameWorld_;
  bool exitRequested_ = false;

 public:
  GameStateManager(GameWorld& gameWorld);
  void pushState(std::unique_ptr<IGameState> state);
  void popState();

  void handleInput(const sf::Event& event);
  void update(float dt);
  void render(sf::RenderWindow& window);

  void requestExit() { exitRequested_ = true; }
  bool isExitRequested() const { return exitRequested_; }
};
