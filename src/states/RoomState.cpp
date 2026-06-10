#include "RoomState.h"

#include "../core/GameStateManager.h"
#include "../core/GameWorld.h"

RoomState::RoomState(GameStateManager& gsm, GameWorld& gameWorld,
                     const BattleConfigEntry& config, int roomIndex, RoomKind kind,
                     bool& isManualMode, FinishCallback onFinished)
    : gsm_(gsm),
      isManualMode_(isManualMode),
      onFinished_(std::move(onFinished)),
      battleRoom_(config, kind, gameWorld.getCurrentParty(), gameWorld.getAssetManager()),
      btnAutoManual_(L"", {1280.f - 210.f, 10.f}, {200.f, 60.f}),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      roomLabel_(font_, "Room " + std::to_string(roomIndex) + "/100", 24U) {
  battleRoom_.setManualMode(isManualMode_);
  refreshButtonLabel();

  roomLabel_.setFillColor(sf::Color::White);
  roomLabel_.setPosition({10.f, 10.f});
}

void RoomState::refreshButtonLabel() {
  btnAutoManual_ = Button(isManualMode_ ? L"Manual" : L"Auto",
                          {1280.f - 210.f, 10.f}, {200.f, 60.f});
  btnAutoManual_.onClick = [this]() {
    isManualMode_ = !isManualMode_;
    battleRoom_.setManualMode(isManualMode_);
    refreshButtonLabel();
  };
}

void RoomState::handleInput(const sf::Event& event) {
  btnAutoManual_.handleInput(event);

  if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (click->button == sf::Mouse::Button::Left)
      battleRoom_.handleClick(sf::Vector2f(click->position));
  }
}

void RoomState::update(float dt) {
  if (finished_) return;

  battleRoom_.update(dt);

  if (battleRoom_.isFinished()) {
    finished_ = true;
    onFinished_(battleRoom_.getCoinsEarned(), battleRoom_.takeLootEarned(),
                battleRoom_.isPlayerWiped());
    gsm_.popState();
  }
}

void RoomState::onResume() {}

void RoomState::render(sf::RenderWindow& window) {
  battleRoom_.render(window);
  btnAutoManual_.render(window);
  window.draw(roomLabel_);
}

const bool RoomState::shouldRenderBelow() const { return false; }

const bool RoomState::shouldUpdateBelow() const { return false; }
