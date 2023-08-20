#pragma once

#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct FrictionComponent {
  float friction = 15;

  bool applyOnX = true;
  bool applyOnY = false;

  sf::Vector2f getApplyVec() { return sf::Vector2f(applyOnX, applyOnY); }

  FrictionComponent() = default;
  FrictionComponent(const FrictionComponent &) = default;
  FrictionComponent(FrictionComponent &) = default;

  FrictionComponent(float f, bool x, bool y)
      : friction(f), applyOnX(x), applyOnY(y) {}

  operator const float &() const { return friction; }
  operator float &() { return friction; }
};
} // namespace Luntik::Components
