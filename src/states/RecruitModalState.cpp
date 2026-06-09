#include "RecruitModalState.h"


RecruitModalState::RecruitModalState(GameStateManager& gsm,
                                     HeroTemplate& heroTemplate,
                                     GameWorld& gameWorld,
                                     std::function<void()> onBought)
    : gsm_(gsm),
      heroTemplate_(heroTemplate),
      gameWorld_(gameWorld),
      onBought_(onBought),
      backgroundSprite_(backgroundTexture_),
      btnBuy_(L"Buy", {10, 10}, {100, 50}, "assets/sprites/ui/button_pink.png",
              true),
      btnClose_(L"Close", {10, 70}, {100, 50},
                "assets/sprites/ui/button_pink.png", true),
      heroSprite_(heroTexture_),
      heroSellFont_("assets/fonts/DelaGothicOne-Regular.ttf"),
      heroNameText_(heroSellFont_, heroTemplate.name, 48U),
      heroPriceText_(heroSellFont_,
                     "Price: " + std::to_string(heroTemplate.price), 42U) {
  static constexpr sf::Vector2f kModalSize = {400.f, 600.f};
  const sf::Vector2f modalPos = {(1280 - kModalSize.x) / 2.f,
                                 (720 - kModalSize.y) / 2.f};

  backgroundTexture_.loadFromFile("assets/sprites/ui/buy_hero_background.png");
  backgroundSprite_.setTexture(backgroundTexture_, true);
  sf::Vector2u bgTexSize = backgroundTexture_.getSize();
  backgroundSprite_.setScale(
      {kModalSize.x / bgTexSize.x, kModalSize.y / bgTexSize.y});
  backgroundSprite_.setPosition(modalPos);

  heroTexture_.loadFromFile(heroTemplate_.spritePath);
  heroSprite_.setTexture(heroTexture_, true);
  heroSprite_.setPosition(
      modalPos + (kModalSize / 2.f) -
      sf::Vector2f{heroSprite_.getTextureRect().size.x / 2.f,
                   heroSprite_.getTextureRect().size.y / 2.f} -
      sf::Vector2f{0.f, 50.f});

  heroNameText_.setPosition({modalPos.x + kModalSize.x / 2.f -
                                 heroNameText_.getLocalBounds().size.x / 2.f,
                             modalPos.y + 15.f});

  heroPriceText_.setPosition({modalPos.x + kModalSize.x / 2.f -
                                  heroPriceText_.getLocalBounds().size.x / 2.f,
                              heroNameText_.getPosition().y +
                                  heroNameText_.getLocalBounds().size.y +
                                  10.f});

  float btnWidth = kModalSize.x - 40.f;
  float btnX = modalPos.x + 20.f;
  float btnY = modalPos.y + kModalSize.y - 120.f - 15.f;

  btnBuy_ = Button(L"Buy", {btnX, btnY}, {btnWidth, 50.f},
                   "assets/sprites/ui/button_pink.png", true);
  btnClose_ = Button(L"Close", {btnX, btnY + 60.f}, {btnWidth, 50.f},
                     "assets/sprites/ui/button_pink.png", true);

  btnClose_.onClick = [&gsm]() { gsm.popState(); };
  btnBuy_.onClick = [this, &gsm]() {
    if (tryBuyHero()) {
      gsm_.popState();
    }
  };
}

void RecruitModalState::handleInput(const sf::Event& event) {
  btnBuy_.handleInput(event);
  btnClose_.handleInput(event);
}

void RecruitModalState::update(float dt) {}

void RecruitModalState::onResume() {}

void RecruitModalState::render(sf::RenderWindow& window) {
  window.draw(backgroundSprite_);

  window.draw(heroSprite_);
  window.draw(heroNameText_);
  window.draw(heroPriceText_);

  btnBuy_.render(window);
  btnClose_.render(window);
}

const bool RecruitModalState::shouldRenderBelow() const { return true; }

const bool RecruitModalState::shouldUpdateBelow() const { return false; }

bool RecruitModalState::tryBuyHero() {
  if (gameWorld_.getCurrentParty().size() < gameWorld_.kMaxSquadSize) {
    if (gameWorld_.tryConsumeCoins(heroTemplate_.price)) {
      if (gameWorld_.tryRecruitHero(heroTemplate_)) {
        if (onBought_) onBought_();
        return true;
      }
    }
  }
  return false;
}
