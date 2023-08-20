#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct VelocityComponent {
  sf::Vector2f velocity;

  VelocityComponent() = default;
  VelocityComponent(const VelocityComponent &) = default;
  VelocityComponent(VelocityComponent &) = default;

  VelocityComponent(float x, float y) : velocity(x, y) {}

  VelocityComponent(const sf::Vector2f &p) : velocity(p){};
  VelocityComponent(sf::Vector2f &p) : velocity(p){};

  operator const sf::Vector2f &() const { return velocity; }
  operator sf::Vector2f &() { return velocity; }
};
} // namespace Luntik::Components
