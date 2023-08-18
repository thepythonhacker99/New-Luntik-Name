#pragma once

#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Luntik::Utils {
template <typename T>
float distanceSquared(sf::Vector2<T> v1, sf::Vector2<T> v2) {
  return std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2);
}

template <typename T> float distance(sf::Vector2<T> v1, sf::Vector2<T> v2) {
  return std::sqrt(distanceSquared(v1, v2));
}

} // namespace Luntik::Utils
