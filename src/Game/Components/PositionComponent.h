#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct PositionComponent {
  sf::Vector2f pos;

  PositionComponent() = default;
  PositionComponent(const PositionComponent &) = default;
  PositionComponent(PositionComponent &) = default;

  PositionComponent(float x, float y) : pos(x, y) {}

  PositionComponent(const sf::Vector2f &p) : pos(p){};
  PositionComponent(sf::Vector2f &p) : pos(p){};

  operator const sf::Vector2f &() const { return pos; }
  operator sf::Vector2f &() { return pos; }
};
} // namespace Luntik::Components
