#include "ItemSelectState.h"

#include <algorithm>

ItemSelectState::ItemSelectState(GameStateManager& gsm, GameWorld& gameWorld,
                                 Hero& hero, bool isWeapon)
    : gsm_(gsm),
      gameWorld_(gameWorld),
      hero_(hero),
      isWeapon_(isWeapon),
      bgSprite_(bgTexture_),
      font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      titleText_(font_, isWeapon ? "Select Weapon" : "Select Armor", 28U),
      emptyText_(font_, "No items available", 22U),
      pageText_(font_, "", 18U),
      btnClose_(L"Close",
                {kWindowX + (kWindowW - 180.f) / 2.f, kWindowY + 428.f},
                {180.f, 48.f}, "assets/sprites/ui/button_pink_item_close.png", true),
      btnPrev_(L"<-",
               {kWindowX + 10.f, kWindowY + 372.f},
               {kBtnW + 20.f, 36.f}, "assets/sprites/ui/button_pink_prevnext.png", true),
      btnNext_(L"->",
               {kWindowX + kWindowW - kBtnW - 30.f, kWindowY + 372.f},
               {kBtnW + 20.f, 36.f}, "assets/sprites/ui/button_pink_prevnext.png", true) {
  overlay_.setSize({1280.f, 720.f});
  overlay_.setFillColor(sf::Color(0, 0, 0, 150));

  bgTexture_.loadFromFile("assets/sprites/ui/manage_hero_bg.png");
  bgSprite_.setTexture(bgTexture_, true);
  sf::Vector2u sz = bgTexture_.getSize();
  bgSprite_.setScale({kWindowW / sz.x, kWindowH / sz.y});
  bgSprite_.setPosition({kWindowX, kWindowY});

  titleText_.setFillColor(sf::Color::White);
  titleText_.setPosition(
      {kWindowX + kWindowW / 2.f - titleText_.getLocalBounds().size.x / 2.f,
       kWindowY + 12.f});

  emptyText_.setFillColor(sf::Color(200, 200, 200));
  emptyText_.setPosition(
      {kWindowX + kWindowW / 2.f - emptyText_.getLocalBounds().size.x / 2.f,
       kWindowY + kWindowH / 2.f - 20.f});

  pageText_.setFillColor(sf::Color::White);

  btnClose_.onClick = [this]() { pendingClose_ = true; };
  btnPrev_.onClick  = [this]() { pendingPrevPage_ = true; };
  btnNext_.onClick  = [this]() { pendingNextPage_ = true; };

  buildItemList();
}

void ItemSelectState::buildItemList() {
  // Filter inventory by slot type
  filteredIndices_.clear();
  auto& inventory = gameWorld_.getInventory();
  const std::string expectedType = isWeapon_ ? "weapon" : "armor";
  for (int i = 0; i < static_cast<int>(inventory.size()); i++) {
    if (inventory[i]->getType() == expectedType)
      filteredIndices_.push_back(i);
  }

  // Clamp page
  int filtTotal = static_cast<int>(filteredIndices_.size());
  int maxPage = filtTotal == 0 ? 0 : (filtTotal - 1) / kItemsPerPage;
  if (currentPage_ > maxPage) currentPage_ = maxPage;
  if (currentPage_ < 0) currentPage_ = 0;

  // Clear UI
  itemLabels_.clear();
  priceLabels_.clear();
  equipButtons_.clear();
  sellButtons_.clear();

  // Page indicator
  int totalPages = filtTotal == 0 ? 1 : (filtTotal + kItemsPerPage - 1) / kItemsPerPage;
  pageText_.setString(std::to_string(currentPage_ + 1) + " / " +
                      std::to_string(totalPages));
  pageText_.setPosition(
      {kWindowX + kWindowW / 2.f - pageText_.getLocalBounds().size.x / 2.f,
       kWindowY + 378.f});

  int startRow = currentPage_ * kItemsPerPage;
  int endRow   = std::min(startRow + kItemsPerPage, filtTotal);

  for (int row = startRow; row < endRow; row++) {
    int invIdx = filteredIndices_[row];
    float y    = kWindowY + 55.f + (row - startRow) * kRowH;

    sf::Text label(font_, inventory[invIdx]->getName(), 19U);
    label.setFillColor(sf::Color::White);
    label.setPosition({kWindowX + 10.f, y + 8.f});
    itemLabels_.push_back(std::move(label));

    const std::string statLabel =
        (inventory[invIdx]->getType() == "weapon" ? "ATK" : "DEF");
    sf::Text price(font_,
                   "sell: " + std::to_string(inventory[invIdx]->getPrice() / 2) +
                       "   " + statLabel + ": +" +
                       std::to_string(inventory[invIdx]->getEffectValue()),
                   13U);
    price.setFillColor(sf::Color(200, 200, 160));
    price.setPosition({kWindowX + 10.f, y + 27.f});
    priceLabels_.push_back(std::move(price));

    // Equip button — right-aligned, gap 5px between buttons
    float sellBtnX  = kWindowX + kWindowW - 40.f - kBtnW;
    float equipBtnX = sellBtnX - 5.f - kBtnW;

    equipButtons_.emplace_back(L"Equip",
                               sf::Vector2f{equipBtnX, y + 5.f},
                               sf::Vector2f{kBtnW, kBtnH},
                               "assets/sprites/ui/button_pink_item_management.png", true);
    equipButtons_.back().onClick = [this, invIdx]() { pendingEquipIndex_ = invIdx; };

    sellButtons_.emplace_back(L"Sell",
                              sf::Vector2f{sellBtnX, y + 5.f},
                              sf::Vector2f{kBtnW, kBtnH},
                              "assets/sprites/ui/button_pink_item_management.png", true);
    sellButtons_.back().onClick = [this, invIdx]() { pendingSellIndex_ = invIdx; };
  }
}

void ItemSelectState::handleInput(const sf::Event& event) {
  btnClose_.handleInput(event);
  btnPrev_.handleInput(event);
  btnNext_.handleInput(event);
  for (auto& btn : equipButtons_) btn.handleInput(event);
  for (auto& btn : sellButtons_)  btn.handleInput(event);

  if (pendingEquipIndex_.has_value()) {
    int idx = pendingEquipIndex_.value();
    pendingEquipIndex_.reset();
    auto& inventory = gameWorld_.getInventory();
    if (idx < static_cast<int>(inventory.size())) {
      auto item = std::move(inventory[idx]);
      inventory.erase(inventory.begin() + idx);
      if (isWeapon_)
        hero_.equipWeapon(std::move(item));
      else
        hero_.equipArmor(std::move(item));
    }
    pendingClose_ = true;
  }

  if (pendingSellIndex_.has_value()) {
    int idx = pendingSellIndex_.value();
    pendingSellIndex_.reset();
    auto& inventory = gameWorld_.getInventory();
    if (idx < static_cast<int>(inventory.size())) {
      gameWorld_.addCoins(inventory[idx]->getPrice() / 2);
      inventory.erase(inventory.begin() + idx);
    }
    buildItemList();
  }

  if (pendingNextPage_) {
    pendingNextPage_ = false;
    int filtTotal = static_cast<int>(filteredIndices_.size());
    int maxPage   = filtTotal == 0 ? 0 : (filtTotal - 1) / kItemsPerPage;
    if (currentPage_ < maxPage) ++currentPage_;
    buildItemList();
  }

  if (pendingPrevPage_) {
    pendingPrevPage_ = false;
    if (currentPage_ > 0) --currentPage_;
    buildItemList();
  }

  if (pendingClose_) {
    gsm_.popState();
    return;
  }
}

void ItemSelectState::update(float dt) {}

void ItemSelectState::onResume() {}

void ItemSelectState::render(sf::RenderWindow& window) {
  window.draw(overlay_);
  window.draw(bgSprite_);
  window.draw(titleText_);

  if (itemLabels_.empty()) {
    window.draw(emptyText_);
  } else {
    for (auto& label : itemLabels_)  window.draw(label);
    for (auto& price : priceLabels_) window.draw(price);
    for (auto& btn : equipButtons_)  btn.render(window);
    for (auto& btn : sellButtons_)   btn.render(window);
  }

  window.draw(pageText_);
  btnPrev_.render(window);
  btnNext_.render(window);
  btnClose_.render(window);
}

const bool ItemSelectState::shouldRenderBelow() const { return true; }
const bool ItemSelectState::shouldUpdateBelow() const { return false; }
