#include "BattleResultState.h"

#include "../core/GameStateManager.h"
#include "../core/GameWorld.h"

BattleResultState::BattleResultState(GameStateManager& gsm, GameWorld& gameWorld,
                                     bool victory, int coinsEarned,
                                     std::vector<std::unique_ptr<Item>> lootEarned)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      coinsEarned_(coinsEarned),
      lootEarned_(std::move(lootEarned)),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      titleText_(font_, victory ? "Victory!" : "Defeat", 48U),
      coinsText_(font_, "Coins: " + std::to_string(coinsEarned_), 28U),
      itemsText_(font_, "Items: " + std::to_string(lootEarned_.size()), 28U),
      btnToTavern_(L"To Tavern", {1280.f / 2.f - 180.f, 480.f}, {360.f, 100.f}) {
  overlay_.setSize({1280.f, 720.f});
  overlay_.setFillColor(sf::Color(0, 0, 0, 180));

  titleText_.setFillColor(sf::Color::White);
  titleText_.setPosition({1280.f / 2.f - titleText_.getLocalBounds().size.x / 2.f, 220.f});

  coinsText_.setFillColor(sf::Color::White);
  coinsText_.setPosition({1280.f / 2.f - coinsText_.getLocalBounds().size.x / 2.f, 320.f});

  itemsText_.setFillColor(sf::Color::White);
  itemsText_.setPosition({1280.f / 2.f - itemsText_.getLocalBounds().size.x / 2.f, 370.f});

  btnToTavern_.onClick = [this]() { pendingToTavern_ = true; };
}

void BattleResultState::handleInput(const sf::Event& event) {
  btnToTavern_.handleInput(event);

  if (pendingToTavern_) {
    gameWorld_.finishRun(coinsEarned_, std::move(lootEarned_));
    gameWorld_.clearParty();
    gsm_.popState();
    return;
  }
}

void BattleResultState::update(float dt) {}

void BattleResultState::onResume() {}

void BattleResultState::render(sf::RenderWindow& window) {
  window.draw(overlay_);
  window.draw(titleText_);
  window.draw(coinsText_);
  window.draw(itemsText_);
  btnToTavern_.render(window);
}

const bool BattleResultState::shouldRenderBelow() const { return false; }

const bool BattleResultState::shouldUpdateBelow() const { return false; }
