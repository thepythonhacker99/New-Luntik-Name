#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct BoundingBoxComponent {
  sf::Vector2f offset;
  sf::Vector2f size;

  sf::Vector2f getTopLeft() {
    return offset + (sf::Vector2f(-size.x / 2, -size.y / 2));
  }
  sf::Vector2f getTopRight() {
    return offset + (sf::Vector2f(size.x / 2, -size.y / 2));
  }
  sf::Vector2f getBottomLeft() {
    return offset + (sf::Vector2f(-size.x / 2, size.y / 2));
  }
  sf::Vector2f getBottomRight() {
    return offset + (sf::Vector2f(size.x / 2, size.y / 2));
  }

  BoundingBoxComponent() = default;
  BoundingBoxComponent(const BoundingBoxComponent &) = default;
  BoundingBoxComponent(BoundingBoxComponent &) = default;

  BoundingBoxComponent(sf::Vector2f size, sf::Vector2f offset = {0, 0})
      : size(size), offset(offset) {}

  operator const sf::Vector2f &() { return size; }
  operator sf::Vector2f &() { return size; }
};
} // namespace Luntik::Components
