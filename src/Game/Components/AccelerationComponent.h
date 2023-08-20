#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct AccelerationComponent {
  float speed = 5000;
  sf::Vector2f direction;

  AccelerationComponent() = default;
  AccelerationComponent(const AccelerationComponent &) = default;
  AccelerationComponent(AccelerationComponent &) = default;

  AccelerationComponent(float x, float y) : direction(x, y) {}

  AccelerationComponent(const sf::Vector2f &p) : direction(p){};
  AccelerationComponent(sf::Vector2f &p) : direction(p){};

  operator const sf::Vector2f &() const { return direction; }
  operator sf::Vector2f &() { return direction; }
};
} // namespace Luntik::Components
