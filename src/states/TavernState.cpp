#include "TavernState.h"

#include <iostream>
#include <random>

TavernState::TavernState(GameStateManager& gsm, GameWorld& gameWorld)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      btnExit_(L"Exit", {10, 10}, {100, 100}),
      btnGoToBattle_(L"", {1280 - 360 - 10, 10}, {360, 100},
                     "assets/sprites/ui/to_battle.png", true),
      btnPartyManagement_(L"Squad", {300, 650}),
      btnUpgradeTavern_(L"Upgrade tavern", {650, 650}),
      backgroundSprite_(backgroundTexture_),
      coinsFont_("assets/fonts/DelaGothicOne-Regular.ttf"),
      coinsText_(coinsFont_, "0", 30U),
      coinsIconSprite_(coinIconTexture_),
      squadUISprite_(squadUITexture_) {
  coinsText_.setPosition(
      {10 + 50 + 10, 720 - coinsText_.getLocalBounds().size.y * 2.f});

  coinIconTexture_.loadFromFile("assets/sprites/ui/icon_coins.png");
  coinsIconSprite_.setTexture(coinIconTexture_, true);
  coinsIconSprite_.setPosition(
      {10, 720 - coinsIconSprite_.getLocalBounds().size.y - 10});

  squadUITexture_.loadFromFile("assets/sprites/ui/squad_ui.png");
  squadUISprite_.setTexture(squadUITexture_, true);
  squadUISprite_.setPosition(
      {1280.f / 2.f - squadUISprite_.getLocalBounds().size.x / 2.f,
       720.f - squadUISprite_.getLocalBounds().size.y - 10.f});

  btnExit_.onClick = [&gsm]() { gsm.requestExit(); };

  backgroundTexture_.loadFromFile("assets/sprites/tavern_background.jpg");
  backgroundSprite_.setTexture(backgroundTexture_, true);
  spawnHeroes();
}

void TavernState::openHeroRecruitWindow(int i) {
  std::cout << "Oppening window for: " << availibleCards_[i].heroTemplate.name
            << " (" << i << ")" << " : " << availibleCards_.size() << std::endl;
  gsm_.pushState(std::make_unique<RecruitModalState>(
      gsm_, availibleCards_[i].heroTemplate, gameWorld_,
      [this, i]() { availibleCards_[i].isBought = true; }));
}

void TavernState::handleInput(const sf::Event& event) {
  btnExit_.handleInput(event);

  for (auto& card : availibleCards_) {
    if (card.isBought) continue;
    if (card.handleInput(event)) break;
  }
}

void TavernState::update(float dt) {}

void TavernState::onResume() {}

void TavernState::render(sf::RenderWindow& window) {
  window.draw(backgroundSprite_);

  std::for_each(availibleCards_.begin(), availibleCards_.end(),
                [&window](HeroRecruitCard& card) {
                  if (!card.isBought) {
                    card.render(window);
                  }
                });

  window.draw(squadUISprite_);
  coinsText_.setString(std::to_string(gameWorld_.getCoins()));
  window.draw(coinsText_);
  window.draw(coinsIconSprite_);
  btnExit_.render(window);
  btnGoToBattle_.render(window);
}

const bool TavernState::shouldRenderBelow() const { return false; }

const bool TavernState::shouldUpdateBelow() const { return false; }

void TavernState::spawnHeroes() {
  auto heroesAvailible = gameWorld_.getAvailibleHeroesForTavern();
  if (heroesAvailible.empty()) return;

  availibleCards_.clear();

  // Первый слот всегда heroesAvailible[0] - бесплатный персонаж
  availibleCards_.push_back(HeroRecruitCard(
      spawnPositions_[0], heroesAvailible[0].spritePath, heroesAvailible[0],
      [this]() { openHeroRecruitWindow(0); }));

  for (int i = 1; i < 7; i++) {
    int idx = rand() % heroesAvailible.size();
    availibleCards_.push_back(HeroRecruitCard(
        spawnPositions_[i], heroesAvailible[idx].spritePath,
        heroesAvailible[idx],
        [this, heroesAvailible, i]() { openHeroRecruitWindow(i); }));
  }
}
