#pragma once

#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Luntik::Components {
struct PositionInterpolatorComponent {
  sf::Vector2f start;
  sf::Vector2f end;
  float t;

  void setGoal(const sf::Vector2f &goal) {
    start = getPos();
    end = goal;
    t = 0;
  }

  sf::Vector2f getPos() {
    return sf::Vector2f(std::lerp(start.x, end.x, t),
                        std::lerp(start.y, end.y, t));
  }

  PositionInterpolatorComponent() = default;
  PositionInterpolatorComponent(const PositionInterpolatorComponent &) =
      default;
  PositionInterpolatorComponent(PositionInterpolatorComponent &) = default;
};
} // namespace Luntik::Components
