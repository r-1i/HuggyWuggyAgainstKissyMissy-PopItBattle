#pragma once

#include <SFML/Graphics.hpp>

class IGameState {
 public:
  virtual void handleInput(const sf::Event& event) = 0;
  virtual void update(float dt) = 0;
  virtual void onResume() = 0;
  virtual void render(sf::RenderWindow& window) = 0;
  virtual const bool shouldRenderBelow() const = 0;
  virtual const bool shouldUpdateBelow() const = 0;
  virtual ~IGameState() = default;
};
