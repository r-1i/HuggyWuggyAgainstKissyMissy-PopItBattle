#pragma once

#include <vector>

#include "core/GameStateManager.h"
#include "core/GameWorld.h"
#include "ui/Button.h"
#include "ui/HeroRecruitCard.h"

class TavernState : public IGameState {
 public:
  TavernState(GameStateManager& gsm, GameWorld& gameWorld);

 private:
  GameStateManager& gsm_;
  GameWorld& gameWorld_;
  std::vector<HeroRecruitCard> availibleCards_;
  Button btnGoToBattle_;
  Button btnUpgradeTavern_;
  Button btnExit_;
  Button btnPartyManagement_;
  sf::Texture backgroundTexture_;
  sf::Sprite backgroundSprite_;

  // Inherited via IGameState
  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
};
