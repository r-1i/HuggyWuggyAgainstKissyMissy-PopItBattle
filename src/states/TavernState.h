#pragma once

#include <vector>

#include "RecruitModalState.h"
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
  sf::Font coinsFont_;
  sf::Text coinsText_;
  sf::Texture coinIconTexture_;
  sf::Sprite coinsIconSprite_;
  sf::Texture squadUITexture_;
  sf::Sprite squadUISprite_;

  const std::vector<sf::Vector2f> spawnPositions_ = {
      {150.f, 470.f}, {370.f, 490.f}, {590.f, 475.f}, {810.f, 470.f},
      {260.f, 390.f}, {480.f, 380.f}, {700.f, 385.f},
  };

  void openHeroRecruitWindow(int i);

  // Inherited via IGameState
  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
  void spawnHeroes();
};
