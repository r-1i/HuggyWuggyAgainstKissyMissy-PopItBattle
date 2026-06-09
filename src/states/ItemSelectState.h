#pragma once

#include <optional>
#include <vector>

#include "IGameState.h"
#include "core/GameStateManager.h"
#include "core/GameWorld.h"
#include "entities/Hero.h"
#include "ui/Button.h"

class ItemSelectState : public IGameState {
 public:
  ItemSelectState(GameStateManager& gsm, GameWorld& gameWorld, Hero& hero,
                  bool isWeapon);

 private:
  GameStateManager& gsm_;
  GameWorld& gameWorld_;
  Hero& hero_;
  bool isWeapon_;

  sf::Texture bgTexture_;
  sf::Sprite bgSprite_;
  sf::RectangleShape overlay_;
  sf::Font font_;
  sf::Text titleText_;
  sf::Text emptyText_;
  sf::Text pageText_;

  std::vector<sf::Text> itemLabels_;
  std::vector<sf::Text> priceLabels_;
  std::vector<Button> equipButtons_;
  std::vector<Button> sellButtons_;
  std::vector<int> filteredIndices_;

  Button btnClose_;
  Button btnPrev_;
  Button btnNext_;

  int currentPage_ = 0;
  std::optional<int> pendingEquipIndex_;
  std::optional<int> pendingSellIndex_;
  bool pendingNextPage_ = false;
  bool pendingPrevPage_ = false;
  bool pendingClose_ = false;

  static constexpr int   kItemsPerPage = 8;
  static constexpr float kRowH         = 44.f;
  static constexpr float kBtnW         = 120.f;
  static constexpr float kBtnH         = 34.f;
  static constexpr float kWindowW      = 500.f;
  static constexpr float kWindowH      = 490.f;
  static constexpr float kWindowX      = (1280.f - kWindowW) / 2.f;
  static constexpr float kWindowY      = (720.f  - kWindowH) / 2.f;

  void buildItemList();

  void handleInput(const sf::Event& event) override;
  void update(float dt) override;
  void onResume() override;
  void render(sf::RenderWindow& window) override;
  const bool shouldRenderBelow() const override;
  const bool shouldUpdateBelow() const override;
};
