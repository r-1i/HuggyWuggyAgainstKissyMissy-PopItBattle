// huggy-pop-it-battle.cpp : Defines the entry point for the application.

#include "huggy-pop-it-battle.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/Game.h"

int main() {
  Game game;
  game.run();

  // sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Huggy Pop-It
  // Battle"); window.setFramerateLimit(60);

  // const int COUNT = 12;
  // const float PI = 3.14159265f;
  // const sf::Vector2f CENTER = {400.f, 300.f};

  // std::vector<sf::CircleShape> orbs(COUNT);
  // for (int i = 0; i < COUNT; ++i) {
  //   orbs[i].setRadius(18.f);
  //   orbs[i].setOrigin({18.f, 18.f});
  //   float hue = (float)i / COUNT;
  //   orbs[i].setFillColor(
  //       sf::Color((uint8_t)(std::sin(hue * 2 * PI) * 127 + 128),
  //                 (uint8_t)(std::sin(hue * 2 * PI + 2.09f) * 127 + 128),
  //                 (uint8_t)(std::sin(hue * 2 * PI + 4.19f) * 127 + 128)));
  // }

  // sf::CircleShape core(14.f);
  // core.setOrigin({14.f, 14.f});
  // core.setPosition(CENTER);
  // core.setFillColor(sf::Color::White);

  // sf::Clock clock;

  // while (window.isOpen()) {
  //   while (const auto event = window.pollEvent()) {
  //     if (event->is<sf::Event::Closed>()) window.close();
  //   }

  //  float t = clock.getElapsedTime().asSeconds();

  //  for (int i = 0; i < COUNT; ++i) {
  //    float angle = (2 * PI * i / COUNT) + t * 1.2f;
  //    float radius = 120.f + std::sin(t * 2.f + i * 0.5f) * 40.f;
  //    orbs[i].setPosition({CENTER.x + std::cos(angle) * radius,
  //                         CENTER.y + std::sin(angle) * radius});
  //    float pulse = std::sin(t * 3.f + i) * 6.f;
  //    orbs[i].setRadius(18.f + pulse);
  //    orbs[i].setOrigin({18.f + pulse, 18.f + pulse});
  //  }

  //  float corePulse = std::sin(t * 4.f) * 6.f;
  //  core.setRadius(14.f + corePulse);
  //  core.setOrigin({14.f + corePulse, 14.f + corePulse});
  //  core.setFillColor(sf::Color((uint8_t)(std::sin(t) * 127 + 128),
  //                              (uint8_t)(std::sin(t + 2.09f) * 127 + 128),
  //                              (uint8_t)(std::sin(t + 4.19f) * 127 + 128)));

  //  window.clear(sf::Color(10, 10, 20));
  //  for (auto& orb : orbs) window.draw(orb);
  //  window.draw(core);
  //  window.display();
  //}

  // return 0;
}
