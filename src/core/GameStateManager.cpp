#include "core/GameStateManager.h"

#include "core/GameWorld.h"

GameStateManager::GameStateManager(GameWorld& gameWorld)
    : gameWorld_(gameWorld) {}

void GameStateManager::pushState(std::unique_ptr<IGameState> state) {
  states_.push(std::move(state));
}

void GameStateManager::popState() {}

void GameStateManager::handleInput(const sf::Event& event) {
  states_.top()->handleInput(event);
}

void GameStateManager::update(float dt) { states_.top().get()->update(dt); }

void GameStateManager::render(sf::RenderWindow& window) {
  states_.top()->render(window);
}
