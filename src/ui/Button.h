#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
 private:
  static constexpr sf::Vector2f kButtonSize{300.f, 100.f};
  static constexpr std::string_view kDefaultTexture =
      "assets/sprites/ui/default_button.png";

  sf::Texture texture_;
  sf::Sprite sprite_;
  sf::Font font_;
  sf::Text text_;

 public:
  Button(const sf::String& text, const sf::Vector2f& position,
         const sf::Vector2f& size = kButtonSize,
         const std::string& textureFileName = std::string(kDefaultTexture),
         bool overrideSize = true);

  std::function<void()> onClick;
  void handleInput(const sf::Event& event);
  void render(sf::RenderWindow& window);
};
