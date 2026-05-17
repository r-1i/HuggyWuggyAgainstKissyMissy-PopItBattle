#include "core/GameStateManager.h"

#include "core/GameWorld.h"

GameStateManager::GameStateManager(GameWorld& gameWorld)
    : gameWorld_(gameWorld) {}

void GameStateManager::pushState(std::unique_ptr<IGameState> state) {
  states_.push_back(std::move(state));
}

void GameStateManager::popState() {
  if (states_.empty()) return;
  states_.pop_back();
  if (!states_.empty()) states_.back()->onResume();
}

void GameStateManager::handleInput(const sf::Event& event) {
  states_[states_.size() - 1]->handleInput(event);
}

void GameStateManager::update(float dt) {
  states_[states_.size() - 1].get()->update(dt);
}

void GameStateManager::render(sf::RenderWindow& window) {
  int from = states_.size() - 1;
  while (from > 0 && states_[from]->shouldRenderBelow()) {
    from--;
  }
  for (int i = from; i < states_.size(); i++) {
    states_[i]->render(window);
  }
}
