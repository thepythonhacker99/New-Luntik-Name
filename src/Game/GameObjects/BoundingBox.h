#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::GameObjects {
struct BoundingBox {
  sf::Vector2f offset;
  sf::Vector2f halfSize;

  sf::Vector2f getTopLeft() const { return offset + -halfSize; }
  sf::Vector2f getTopRight() const {
    return offset + (sf::Vector2f(halfSize.x, -halfSize.y));
  }
  sf::Vector2f getBottomLeft() const {
    return offset + (sf::Vector2f(-halfSize.x, halfSize.y));
  }
  sf::Vector2f getBottomRight() const { return offset + halfSize; }

  float getLeft() const { return offset.x - halfSize.x; }
  float getRight() const { return offset.x + halfSize.x; }
  float getTop() const { return offset.y - halfSize.y; }
  float getBottom() const { return offset.y + halfSize.y; }

  BoundingBox expanded(sf::Vector2f size) const {
    return BoundingBox(halfSize + size, offset);
  }

  BoundingBox() = default;
  BoundingBox(const BoundingBox &) = default;
  BoundingBox(BoundingBox &) = default;

  BoundingBox(sf::Vector2f sizeHalf, sf::Vector2f offset = {0, 0})
      : halfSize(sizeHalf), offset(offset) {}

  operator const sf::Vector2f &() const { return halfSize; }
  operator sf::Vector2f &() { return halfSize; }
};
} // namespace Luntik::GameObjects
