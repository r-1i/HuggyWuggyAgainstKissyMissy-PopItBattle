#pragma once

#include <optional>
#include <vector>

#include "IGameState.h"
#include "core/GameStateManager.h"
#include "core/GameWorld.h"
#include "ui/Button.h"
#include "ui/HeroManageCard.h"

class PartyManagementState : public IGameState {
 public:
  PartyManagementState(GameStateManager& gsm, GameWorld& gameWorld);

 private:
  GameStateManager& gsm_;
  GameWorld& gameWorld_;
  sf::Texture bgTexture_;
  sf::Sprite bgSprite_;
  sf::RectangleShape overlay_;
  std::vector<HeroManageCard> cards_;
  Button btnClose_;
  sf::Font font_;
  sf::Text emptyText_;
  std::optional<int> pendingSellIndex_;
  std::optional<int> pendingSlotHeroIndex_;
  bool pendingSlotIsWeapon_ = false;
  bool pendingClose_ = false;

  static constexpr float kWindowW = 650.f;
  static constexpr float kWindowH = 720.f;
  static constexpr float kWindowX = (1280.f - kWindowW) / 2.f;
  static constexpr float kWindowY = 0.f;
  static constexpr float kPad = 10.f;
  static constexpr float kCardH = 110.f;
  static constexpr float kCardStep = kCardH + kPad;

  void refresh();

  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
};
