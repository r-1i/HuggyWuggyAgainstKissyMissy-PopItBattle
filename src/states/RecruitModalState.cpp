#include "RecruitModalState.h"

RecruitModalState::RecruitModalState(GameStateManager& gsm,
                                     HeroTemplate& heroTemplate,
                                     GameWorld& gameWorld)
    : gsm_(gsm),
      heroTemplate_(heroTemplate),
      gameWorld_(gameWorld),
      btnBuy_(L"Exit", {10, 10}, {100, 100}),
      btnClose_(L"Close", {10, 10}, {100, 100}),
      heroSprite_(heroTexture_),
      heroSellFont_("assets/fonts/DelaGothicOne-Regular.ttf"),
      heroNameText_(heroSellFont_, heroTemplate.name, 48U),
      heroPriceText_(heroSellFont_,
                     "Price: " + std::to_string(heroTemplate.price), 42U) {
  background_.setFillColor(sf::Color(100, 100, 100, 255));
  background_.setPosition({(1280 - 400) / 2.f, (720 - 600) / 2.f});
  background_.setSize({400, 600});

  heroTexture_.loadFromFile(heroTemplate_.spritePath);
  heroSprite_.setTexture(heroTexture_, true);
  heroSprite_.setPosition(
      background_.getPosition() + (background_.getSize() / 2.f) -
      sf::Vector2f{heroSprite_.getTextureRect().size.x / 2.f,
                   heroSprite_.getTextureRect().size.y / 2.f} -
      sf::Vector2f{0.f, 50.f});

  heroNameText_.setPosition({background_.getPosition().x +
                                 background_.getSize().x / 2.f -
                                 heroNameText_.getLocalBounds().size.x / 2.f,
                             background_.getPosition().y + 15.f});

  heroPriceText_.setPosition(
      {background_.getPosition().x + background_.getSize().x / 2.f -
           heroPriceText_.getLocalBounds().size.x / 2.f,
       heroNameText_.getPosition().y + heroNameText_.getLocalBounds().size.y +
           10.f});

  float btnWidth = heroNameText_.getLocalBounds().size.x;
  float btnX = background_.getPosition().x + background_.getSize().x / 2.f -
               btnWidth / 2.f;
  float btnY =
      background_.getPosition().y + background_.getSize().y - 120.f - 15.f;

  btnBuy_ = Button(L"Buy", {btnX, btnY}, {btnWidth, 50.f});
  btnClose_ = Button(L"Close", {btnX, btnY + 60.f}, {btnWidth, 50.f});

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
  window.draw(background_);

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
        return true;
      }
    }
  }
  return false;
}
