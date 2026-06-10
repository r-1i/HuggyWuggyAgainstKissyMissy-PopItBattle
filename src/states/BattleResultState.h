#pragma once

#include <memory>
#include <vector>

#include "IGameState.h"
#include "../items/Item.h"
#include "../ui/Button.h"

class GameStateManager;
class GameWorld;

class BattleResultState : public IGameState {
 public:
  BattleResultState(GameStateManager& gsm, GameWorld& gameWorld, bool victory,
                     int coinsEarned, std::vector<std::unique_ptr<Item>> lootEarned);

  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;

 private:
  GameStateManager& gsm_;
  GameWorld& gameWorld_;

  int coinsEarned_;
  std::vector<std::unique_ptr<Item>> lootEarned_;
  bool pendingToTavern_ = false;

  sf::RectangleShape overlay_;
  sf::Font font_;
  sf::Text titleText_;
  sf::Text coinsText_;
  sf::Text itemsText_;
  Button btnToTavern_;
};
