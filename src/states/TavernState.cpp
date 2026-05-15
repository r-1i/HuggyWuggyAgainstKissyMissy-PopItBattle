#include "TavernState.h"

TavernState::TavernState(GameStateManager& gsm, GameWorld& gameWorld)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      btnExit_(L"Exit", {300, 300}),
      btnGoToBattle_(L"Бой!", {650, 300}),
      btnPartyManagement_(L"Squad", {300, 650}),
      btnUpgradeTavern_(L"Upgrade tavern", {650, 650}),
      backgroundSprite_(backgroundTexture_) {
  btnExit_.onClick = [&gsm]() { gsm.requestExit(); };
  backgroundTexture_.loadFromFile("assets/sprites/tavern_background.jpg");
  backgroundSprite_.setTexture(backgroundTexture_, true);
}

void TavernState::handleInput(const sf::Event& event) {
  btnExit_.handleInput(event);
}

void TavernState::update(float dt) {}

void TavernState::onResume() {}

void TavernState::render(sf::RenderWindow& window) {
  window.draw(backgroundSprite_);

  btnExit_.render(window);
  btnGoToBattle_.render(window);
}

const bool TavernState::shouldRenderBelow() const { return false; }

const bool TavernState::shouldUpdateBelow() const { return false; }
