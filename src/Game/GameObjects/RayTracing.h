#pragma once

#include "BoundingBox.h"
#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Luntik::RayTracing {
struct Ray {
  sf::Vector2f origin;
  sf::Vector2f direction;
};

struct RayTraceResult {
  float t;
  sf::Vector2f normal;

  bool hasCollision = false;
};

RayTraceResult rayVsBoundingBox(Ray ray, GameObjects::BoundingBox box,
                                sf::Vector2f pos);
} // namespace Luntik::RayTracing
