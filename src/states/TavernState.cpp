#include "TavernState.h"

#include <algorithm>
#include <iostream>
#include <random>

#include "BattleState.h"
#include "PartyManagementState.h"

TavernState::TavernState(GameStateManager& gsm, GameWorld& gameWorld)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      btnExit_(L"Exit", {10, 10}, {100, 100}),
      btnGoToBattle_(L"", {1280 - 360 - 10, 720 - 100 - 10}, {360, 100},
                     "assets/sprites/ui/to_battle.png", true),
      btnPartyManagement_(L"", {10, 720 - 100 - 10}, {360, 100},
                          "assets/sprites/ui/manage_squad.png", true),
      btnUpgradeTavern_(L"Upgrade tavern", {650, 650}),
      backgroundSprite_(backgroundTexture_),
      coinsFont_("assets/fonts/DelaGothicOne-Regular.ttf"),
      coinsText_(coinsFont_, "0", 30U),
      coinsIconSprite_(coinIconTexture_),
      coinsBgSprite_(coinsBgTexture_),
      squadUISprite_(squadUITexture_) {
  coinIconTexture_.loadFromFile("assets/sprites/ui/icon_coins.png");
  coinsIconSprite_.setTexture(coinIconTexture_, true);

  coinsBgTexture_.loadFromFile("assets/sprites/ui/coins_background.png");
  coinsBgSprite_.setTexture(coinsBgTexture_, true);
  {
    static constexpr float kBgW = 280.f;
    static constexpr float kBgH = 100.f;
    sf::Vector2u sz = coinsBgTexture_.getSize();
    coinsBgSprite_.setScale({kBgW / sz.x, kBgH / sz.y});
    coinsBgSprite_.setPosition({(1280.f - kBgW) / 2.f, 0.f});
  }

  squadUITexture_.loadFromFile("assets/sprites/ui/squad_ui.png");
  squadUISprite_.setTexture(squadUITexture_, true);
  squadUISprite_.setPosition(
      {1280.f / 2.f - squadUISprite_.getLocalBounds().size.x / 2.f,
       720.f - squadUISprite_.getLocalBounds().size.y - 10.f});

  btnExit_.onClick = [&gsm]() { gsm.requestExit(); };
  btnPartyManagement_.onClick = [&gsm, &gameWorld]() {
    gsm.pushState(std::make_unique<PartyManagementState>(gsm, gameWorld));
  };
  btnGoToBattle_.onClick = [&gsm, &gameWorld]() {
    gsm.pushState(std::make_unique<BattleState>(gsm, gameWorld));
  };

  backgroundTexture_.loadFromFile("assets/sprites/tavern_background.jpg");
  backgroundSprite_.setTexture(backgroundTexture_, true);
  spawnHeroes();

  partyIconTextures_.reserve(gameWorld_.kMaxSquadSize);
  refreshPartySlots();
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
  btnGoToBattle_.handleInput(event);
  btnPartyManagement_.handleInput(event);

  for (auto& card : availibleCards_) {
    if (card.isBought) continue;
    if (card.handleInput(event)) break;
  }
}

void TavernState::update(float dt) {}

void TavernState::onResume() {
  spawnHeroes();
  refreshPartySlots();
}

void TavernState::render(sf::RenderWindow& window) {
  window.draw(backgroundSprite_);

  std::for_each(availibleCards_.begin(), availibleCards_.end(),
                [&window](HeroRecruitCard& card) {
                  if (!card.isBought) {
                    card.render(window);
                  }
                });

  window.draw(squadUISprite_);

  for (auto& sprite : partyIconSprites_) window.draw(sprite);

  window.draw(coinsBgSprite_);

  coinsText_.setString(std::to_string(gameWorld_.getCoins()));
  float iconW = coinsIconSprite_.getLocalBounds().size.x;
  float iconH = coinsIconSprite_.getLocalBounds().size.y;
  float totalW = iconW + 10.f + coinsText_.getLocalBounds().size.x;
  float coinsY = coinsBgSprite_.getGlobalBounds().size.y / 2.f - iconH / 2.f;
  coinsIconSprite_.setPosition({(1280.f - totalW) / 2.f - 20.f, coinsY});
  coinsText_.setPosition(
      {(1280.f - totalW) / 2.f + iconW + 0.f,
       coinsY - 5.f + (iconH - coinsText_.getLocalBounds().size.y) / 2.f});
  window.draw(coinsIconSprite_);
  window.draw(coinsText_);
  btnExit_.render(window);
  btnGoToBattle_.render(window);
  btnPartyManagement_.render(window);
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

void TavernState::refreshPartySlots() {
  partyIconTextures_.clear();
  partyIconSprites_.clear();

  auto& party = gameWorld_.getCurrentParty();
  std::size_t slotCount = std::min(party.size(), partySlotCenters_.size());

  for (std::size_t i = 0; i < slotCount; i++) {
    if (party[i].iconPath.empty()) continue;

    sf::Texture texture;
    if (!texture.loadFromFile(party[i].iconPath)) continue;

    partyIconTextures_.push_back(std::move(texture));
    sf::Texture& loadedTexture = partyIconTextures_.back();

    sf::Sprite sprite(loadedTexture);
    sf::Vector2u textureSize = loadedTexture.getSize();
    float scale = kPartyIconSize /
                  static_cast<float>(std::max(textureSize.x, textureSize.y));
    sprite.setScale({scale, scale});
    sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
    sprite.setPosition(squadUISprite_.getPosition() + partySlotCenters_[i]);

    partyIconSprites_.push_back(std::move(sprite));
  }
}
