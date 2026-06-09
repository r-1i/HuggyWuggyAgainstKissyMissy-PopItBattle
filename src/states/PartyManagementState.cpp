#include "PartyManagementState.h"

#include "ItemSelectState.h"

PartyManagementState::PartyManagementState(GameStateManager& gsm,
                                           GameWorld& gameWorld)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      bgSprite_(bgTexture_),
      btnClose_(L"Close",
                {kWindowX + (kWindowW - 360.f) / 2.f, kWindowY + 650.f},
                {360.f, 50.f}, "assets/sprites/ui/button_pink.png", true),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      emptyText_(font_, "No heroes in your squad", 28U) {
  overlay_.setSize({1280.f, 720.f});
  overlay_.setFillColor(sf::Color(0, 0, 0, 120));

  bgTexture_.loadFromFile("assets/sprites/ui/manage_hero_bg.png");
  bgSprite_.setTexture(bgTexture_, true);
  sf::Vector2u sz = bgTexture_.getSize();
  bgSprite_.setScale({kWindowW / sz.x, kWindowH / sz.y});
  bgSprite_.setPosition({kWindowX, kWindowY});

  emptyText_.setFillColor(sf::Color::White);
  emptyText_.setPosition(
      {kWindowX + kWindowW / 2.f - emptyText_.getLocalBounds().size.x / 2.f,
       kWindowY + kWindowH / 2.f - 40.f});

  btnClose_.onClick = [this]() { pendingClose_ = true; };

  refresh();
}

void PartyManagementState::refresh() {
  cards_.clear();
  auto& party = gameWorld_.getCurrentParty();
  int n = static_cast<int>(party.size());
  if (n == 0) return;

  cards_.reserve(n);
  for (int i = 0; i < n; i++) {
    float x = kWindowX + kPad;
    float y = kWindowY + kPad + i * kCardStep;
    cards_.emplace_back(sf::Vector2f{x, y}, party[i],
                        [this, i]() { pendingSellIndex_ = i; },
                        [this, i](bool isWeapon) {
                          pendingSlotHeroIndex_ = i;
                          pendingSlotIsWeapon_ = isWeapon;
                        });
  }
}

void PartyManagementState::handleInput(const sf::Event& event) {
  btnClose_.handleInput(event);
  for (auto& card : cards_) card.handleInput(event);

  if (pendingSellIndex_.has_value()) {
    gameWorld_.sellHero(pendingSellIndex_.value());
    pendingSellIndex_.reset();
    refresh();
  }

  if (pendingSlotHeroIndex_.has_value()) {
    int idx = pendingSlotHeroIndex_.value();
    bool isWeapon = pendingSlotIsWeapon_;
    pendingSlotHeroIndex_.reset();
    auto& hero = gameWorld_.getCurrentParty()[idx];
    const Item* equipped = isWeapon ? hero.getWeapon() : hero.getArmor();
    if (equipped) {
      auto item = isWeapon ? hero.unequipWeapon() : hero.unequipArmor();
      gameWorld_.addItem(std::move(item));
      refresh();
    } else {
      gsm_.pushState(
          std::make_unique<ItemSelectState>(gsm_, gameWorld_, hero, isWeapon));
    }
  }

  if (pendingClose_) {
    gsm_.popState();
    return;
  }
}

void PartyManagementState::update(float dt) {}

void PartyManagementState::onResume() { refresh(); }

void PartyManagementState::render(sf::RenderWindow& window) {
  window.draw(overlay_);
  window.draw(bgSprite_);

  if (cards_.empty()) {
    window.draw(emptyText_);
  } else {
    for (auto& card : cards_) card.render(window);
  }

  btnClose_.render(window);
}

const bool PartyManagementState::shouldRenderBelow() const { return true; }
const bool PartyManagementState::shouldUpdateBelow() const { return false; }
