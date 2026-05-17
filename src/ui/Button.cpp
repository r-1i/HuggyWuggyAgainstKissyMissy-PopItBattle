#include "Button.h"

Button::Button(const sf::String& text, const sf::Vector2f& position,
               const sf::Vector2f& size, const std::string& textureFileName,
               bool overrideSize)
    : font_("assets/fonts/DelaGothicOne-Regular.ttf"),
      text_(font_, text, 30),
      sprite_(texture_) {
  texture_.loadFromFile(textureFileName);
  sprite_.setTexture(texture_);

  if (overrideSize) {
    sprite_.setTextureRect(sf::IntRect{{0, 0}, {(int)size.x, (int)size.y}});
  }

  text_.setFont(font_);
  sf::FloatRect bounds = text_.getLocalBounds();
  text_.setPosition(position + (size / 2.f) - (bounds.size / 2.f) -
                    sf::Vector2f{0.f, 5.f});
  sprite_.setPosition(position);
}

Button::Button(const Button& other)

    : texture_(other.texture_),
      sprite_(texture_),
      font_(other.font_),
      text_(other.text_),
      onClick(other.onClick) {
  sprite_.setPosition(other.sprite_.getPosition());
  text_.setFont(font_);
}

Button& Button::operator=(const Button& other) {
  texture_ = other.texture_;
  font_ = other.font_;
  text_ = other.text_;
  onClick = other.onClick;
  sprite_ = sf::Sprite(texture_);
  sprite_.setPosition(other.sprite_.getPosition());
  sprite_.setTextureRect(other.sprite_.getTextureRect());
  sprite_.setScale(other.sprite_.getScale());
  text_.setFont(font_);
  return *this;
}

bool Button::handleInput(const sf::Event& event) {
  if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (click->button == sf::Mouse::Button::Left) {
      if (sprite_.getGlobalBounds().contains(sf::Vector2f(click->position))) {
        if (onClick) onClick();
        return true;
      }
    }
  }
  return false;
}

void Button::render(sf::RenderWindow& window) {
  window.draw(sprite_);
  window.draw(text_);
}
