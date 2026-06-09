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
  sf::Texture coinsBgTexture_;
  sf::Sprite coinsBgSprite_;
  sf::Texture squadUITexture_;
  sf::Sprite squadUISprite_;

  std::vector<sf::Texture> partyIconTextures_;
  std::vector<sf::Sprite> partyIconSprites_;

  // Slots centers on squad_ui.png (450x150) from top left angle.
  static constexpr float kPartyIconSize = 56.f;
  const std::vector<sf::Vector2f> partySlotCenters_ = {
      {57.f, 86.f}, {139.f, 86.f}, {222.f, 86.f}, {304.f, 86.f}, {386.f, 86.f},
  };

  const std::vector<sf::Vector2f> spawnPositions_ = {
      {175.f, 415.f},  // left floor, near pop-it stand
      {435.f, 445.f},  // center-left floor
      {890.f, 430.f},  // right floor
      {1060.f, 400.f}, // far right floor
      {275.f, 400.f},  // back left, behind pop-it stand
      {530.f, 390.f},  // back center, near box stand
      {760.f, 418.f},  // back right
  };

  void openHeroRecruitWindow(int i);
  void refreshPartySlots();

  // Inherited via IGameState
  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
  void spawnHeroes();
};
