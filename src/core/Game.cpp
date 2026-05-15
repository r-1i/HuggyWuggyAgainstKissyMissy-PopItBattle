#include "Game.h"

#include <memory>

#include "states/TavernState.h"

Game::Game()
    : renderWindow_(sf::VideoMode({1280, 720}), "Huggy-Wuggy"),
      gsm_(gameWorld_) {
  gsm_.pushState(std::make_unique<TavernState>(gsm_, gameWorld_));
}

void Game::run() {
  sf::Clock t;
  while (renderWindow_.isOpen() && !gsm_.isExitRequested()) {
    while (const auto event = renderWindow_.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        renderWindow_.close();
      } else {
        gsm_.handleInput(*event);
      }
    }

    gsm_.update(t.restart().asSeconds());

    renderWindow_.clear();
    gsm_.render(renderWindow_);
    renderWindow_.display();
  }
}
