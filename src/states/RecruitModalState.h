#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "IGameState.h"
#include "core/GameStateManager.h"
#include "entities/HeroTemplate.h"
#include "ui/Button.h"

class RecruitModalState : public IGameState {
 public:
  RecruitModalState(GameStateManager& gsm, HeroTemplate& heroTemplate,
                    GameWorld& gameWorld, std::function<void()> onBought);

 private:
  GameStateManager& gsm_;
  HeroTemplate& heroTemplate_;
  GameWorld& gameWorld_;
  std::function<void()> onBought_;

  sf::RectangleShape background_;
  Button btnClose_;
  Button btnBuy_;
  sf::Texture heroTexture_;
  sf::Sprite heroSprite_;

  sf::Font heroSellFont_;
  sf::Text heroNameText_;
  sf::Text heroPriceText_;

  bool tryBuyHero();

  // Inherited via IGameState
  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
};
